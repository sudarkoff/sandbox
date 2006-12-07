-- http://cale.yi.org/autoshare/FourFour.hs
module FourFour where
import Data.Ratio
import Data.List 
import Control.Monad

data Expr = Const Rational 
          | Expr :+: Expr
          | Expr :*: Expr
          | Expr :-: Expr
          | Expr :/: Expr
          | Expr :^: Expr
    deriving (Eq, Ord)

instance Show Expr where
    show x = case x of
        Const k -> case lookup k pretty of
                     Just s  -> s
                     Nothing -> show k
        x :+: y -> showOp " + " x y
        x :-: y -> showOp " - " x y
        x :*: y -> showOp " * " x y
        x :/: y -> showOp " / " x y
        x :^: y -> showOp " ^ " x y
      where showOp s x y = concat ["(", show x, s, show y, ")"]
            pretty = [(4,"4"), (2,"sqrt 4"), (24, "4!"), (4/10, ".4"),
                      (4/9, ".(4)"), (2/3, "sqrt .(4)")]

-- Monadic evaluator, handles failure cases.
eval (Const r) = return r
eval (x :+: y) = liftM2 (+) (eval x) (eval y)
eval (x :*: y) = liftM2 (*) (eval x) (eval y)
eval (x :-: y) = liftM2 (-) (eval x) (eval y)
eval (x :/: y) = do x' <- eval x
                    y' <- eval y
                    guard $ y' /= 0
                    return (x' / y')
eval (x :^: y) = do x' <- eval x
                    y' <- eval y
                    case y' of
                        2 -> return (x'^2)
                        4 -> return (x'^4)
                        _ -> mzero

-- Build expression trees recursively.
-- Always chooses smaller left subtrees for commutative operators.
buildExprs 0 xs = xs
buildExprs n xs = do 
        k <- [0..n-1]
        e1 <- buildExprs k xs
        e2 <- buildExprs (n-k-1) xs
        op <- if k <= n-k-1
                then [(:+:), (:*:), (:-:), (:/:), (:^:)]
                else [(:-:), (:/:), (:^:)]
        return (e1 `op` e2)

fourFours = buildExprs 3 constants 
  where constants = [Const 4, Const 2, Const 24, Const (4/10),
                     Const (4/9), Const (2/3)]

evals = [(n,x) | x <- fourFours,
                 q <- eval x,
                 denominator q == 1,
                 let n = numerator q,
                 n > 0, n <= 100]

main = mapM solve [1..100]
  where solve n = do
            let Just x = lookup n evals
            putStrLn $ show n ++ ": " ++ show x
