{ nixpkgs, idris, flake-utils, system, stdenv, runCommand, lib }:
let
  withTests = tests: drv:
    let
      testDrvs = lib.mapAttrs (name: testScript:
        runCommand "${drv.name}-test-${name}" { } ''
          ${testScript}
          touch "$out"
        '') tests;
    in testDrvs;
  createTemplate = flake: inputs: type:
    let
      self = import flake;
      template =
        self.outputs ({ inherit self nixpkgs idris flake-utils; } // inputs);
      templateBuild = template.packages.${system}.${type};
    in templateBuild;

  templateBuildDefault = createTemplate ./templates/pkg/flake.nix { } "executable";
  templateBuildDefaultLibrary =
    createTemplate ./templates/pkg/flake.nix { } "library" { };
  templateBuildWithDeps = createTemplate ./templates/pkgWithDeps/flake.nix {
    pkg = templateBuildDefaultLibrary;
  } "executable";

  testsTemplate = {
    checkFoo = ''
      ${templateBuildDefault}/bin/runMyPkg \
        | grep "Foo"
    '';
  };
  testsTemplateWithDeps = {
    checkBar = ''
      ${templateBuildWithDeps}/bin/runMyPkg2 \
        | grep "Bar"
    '';
  };
in withTests testsTemplate templateBuildDefault
// withTests testsTemplateWithDeps templateBuildWithDeps // {
  idris2Tests =
    idris.packages.${system}.default.overrideAttrs (a: { doCheck = true; });
}
