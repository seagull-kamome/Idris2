

case001 : Maybe Int -> List Int -> () -> String
case001 mb lst u =
  (case mb of
    Just _ => "Just"
    Nothing => "Nothing") ++
  (case lst of
    [] => "Nil"
    x::xs => "Cons") ++
  (case u of
    () => "Unit")


case002 : Maybe Int -> List Int -> () -> (Maybe String, List String, ())
case002 mb lst u = (mb' mb, lst' lst, u' u) where
  mb' : Maybe Int -> Maybe String
  mb' = \case
    Just x => Just "Just"
    Nothing => Nothing
  lst' : List Int -> List String
  lst' = \case
    x::xs => "Cons"::(lst' xs)
    [] => []
  u' : Unit -> Unit
  u' = \case
    () => ()

case003 : Maybe Int -> List Int -> () -> (Maybe Int, List Int, ())
case003 mb lst u = (mb' mb, lst' lst, u' u) where
  mb' : Maybe Int -> Maybe Int
  mb' = \case
    Just x => Just x
    Nothing => Nothing
  lst' : List Int -> List Int
  lst' = \case
    x::xs => x::(lst' xs)
    [] => []
  u' : Unit -> Unit
  u' = \case
    () => ()


main : IO ()
main = do
  print $ case001 (Just 1) [1,2,3] ()
  print $ case001 Nothing [] ()
  print $ case002 (Just 1) [1,2,3] ()
  print $ case002 Nothing [] ()
  print $ case003 (Just 1) [1,2,3] ()



