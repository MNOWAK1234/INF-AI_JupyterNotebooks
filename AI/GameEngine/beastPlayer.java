package put.ai.games.players;

import java.util.List;
import put.ai.games.game.Board;
import put.ai.games.game.Move;
import put.ai.games.game.Player;

import static put.ai.games.game.Player.Color.*;

public class BeastPlayer extends Player {

    private Color me, opponent;
    private long timeLimit;
    private int MaxDepth;
    private int size, half, forWin, pivot, toSide;
    private int[][] hoshi = new int[4][2];
    private int opponentSmallX, opponentSmallY;
    private int opponentBigX, opponentBigY;
    private int meSmallX, meSmallY;
    private int meBigX, meBigY;
    private int LineX, LineY;
    // private int[][] preference = new int[900][2];
    private String moveAsStr;

    @Override
    public String getName() {
        return "Beast Player: Champion PUT 2024";
    }

    public static String StringMove(int x, int y) {
        return "Place@(" + x + ", " + y + ")";
    }

    public void assign(Color forWho, int tempx, int tempy) {
        if (forWho == opponent) {
            opponentSmallX = tempx;
            opponentSmallY = tempy;
        } else {
            meSmallX = tempx;
            meSmallY = tempy;
        }
    }

    public void assignBig(Color forWho, int tempx, int tempy) {
        if (forWho == opponent) {
            opponentBigX = tempx;
            opponentBigY = tempy;
        } else {
            meBigX = tempx;
            meBigY = tempy;
        }
    }

    public void SmallThreats(Board b, Color forWho, Color enemy) {
        int centerx;
        int centery;
        int count;
        int tempx = -1, tempy = -1;
        for (int q = 0; q < 4; q++) {
            centerx = hoshi[q][0];
            centery = hoshi[q][1];
            for (int s = 0; s <= toSide; s++) {
                // down
                count = 0;
                for (int i = 0; i <= toSide; i++) {
                    if (b.getState(centerx + s, centery + i) == enemy) {
                        count = 0;
                        break;
                    } else if (b.getState(centerx + s, centery + i) == forWho) {
                        count++;
                    } else {
                        if (i > count)
                            break;
                        tempx = centerx + s;
                        tempy = centery + i;
                    }
                }
                if (count == toSide) {
                    assign(forWho, tempx, tempy);
                    return;
                }
                // up
                count = 0;
                for (int i = 0; i <= toSide; i++) {
                    if (b.getState(centerx + s, centery - i) == enemy) {
                        count = 0;
                        break;
                    }
                    if (b.getState(centerx + s, centery - i) == forWho) {
                        count++;
                    } else {
                        if (i > count)
                            break;
                        tempx = centerx + s;
                        tempy = centery - i;
                    }
                }
                if (count == toSide) {
                    assign(forWho, tempx, tempy);
                    return;
                }
                // right
                count = 0;
                for (int i = 0; i <= toSide; i++) {
                    if (b.getState(centerx + i, centery + s) == enemy) {
                        count = 0;
                        break;
                    }
                    if (b.getState(centerx + i, centery + s) == forWho) {
                        count++;
                    } else {
                        if (i > count)
                            break;
                        tempx = centerx + i;
                        tempy = centery + s;
                    }
                }
                if (count == toSide) {
                    assign(forWho, tempx, tempy);
                    return;
                }
                // left
                count = 0;
                for (int i = 0; i <= toSide; i++) {
                    if (b.getState(centerx - i, centery + s) == enemy) {
                        count = 0;
                        break;
                    }
                    if (b.getState(centerx - i, centery + s) == forWho) {
                        count++;
                    } else {
                        if (i > count)
                            break;
                        tempx = centerx - i;
                        tempy = centery + s;
                    }
                }
                if (count == toSide) {
                    assign(forWho, tempx, tempy);
                    return;
                }
            }
        }
    }

    public void BigThreats(Board b, Color forWho, Color enemy) {
        int centerx;
        int centery;
        int count;
        int tempx = -1, tempy = -1;
        for (int q = 0; q < 4; q++) {
            centerx = hoshi[q][0];
            centery = hoshi[q][1];
            for (int s = 0; s <= toSide; s++) {
                // vertically
                count = 0;
                for (int i = 1; i <= toSide; i++) {
                    // down
                    if (b.getState(centerx + s, centery + i) == enemy) {
                        count = 0;
                        break;
                    } else if (b.getState(centerx + s, centery + i) == forWho) {
                        count++;
                    } else {
                        if (i > count)
                            break;
                        tempx = centerx + s;
                        tempy = centery + i;
                    }
                    // up
                    if (b.getState(centerx + s, centery - i) == enemy) {
                        count = 0;
                        break;
                    }
                    if (b.getState(centerx + s, centery - i) == forWho) {
                        count++;
                    } else {
                        if (2 * i - 1 > count)
                            break;
                        tempx = centerx + s;
                        tempy = centery - i;
                    }
                }
                if (b.getState(centerx + s, centery) == enemy)
                    count = 0;
                else if (b.getState(centerx + s, centery) == forWho)
                    count++;
                else {
                    tempx = centerx + s;
                    tempy = centery;
                }
                if (count == 2 * toSide) {
                    assignBig(forWho, tempx, tempy);
                    return;
                }
                // horizontally
                count = 0;
                for (int i = 1; i <= toSide; i++) {
                    // right
                    if (b.getState(centerx + i, centery + s) == enemy) {
                        count = 0;
                        break;
                    }
                    if (b.getState(centerx + i, centery + s) == forWho) {
                        count++;
                    } else {
                        if (i > count)
                            break;
                        tempx = centerx + i;
                        tempy = centery + s;
                    }
                    // left
                    if (b.getState(centerx - i, centery + s) == enemy) {
                        count = 0;
                        break;
                    }
                    if (b.getState(centerx - i, centery + s) == forWho) {
                        count++;
                    } else {
                        if (2 * i - 1 > count)
                            break;
                        tempx = centerx - i;
                        tempy = centery + s;
                    }
                }
                if (b.getState(centerx, centery + s) == enemy)
                    count = 0;
                else if (b.getState(centerx, centery + s) == forWho)
                    count++;
                else {
                    tempx = centerx;
                    tempy = centery + s;
                }
                if (count == 2 * toSide) {
                    assignBig(forWho, tempx, tempy);
                    return;
                }
            }
        }
    }

    public void Line(Board b) {
        for (int i = 0; i < size; i++) {
            ;
        }
    }

    private int scoreOpponent(Board b, int depth, int alpha) {
        Color winnerColor = b.getWinner(opponent);
        if (winnerColor == me)
            return -10000;
        else if (winnerColor == opponent)
            return 10000;
        else if (winnerColor == EMPTY)
            return 0;
        // Check if maxDepth was reached
        if (depth == MaxDepth)
            return 0;
        if (System.currentTimeMillis() >= timeLimit)
            return 0;
        // Add best moves from previous iteration
        // if (moveMapping.containsKey(globalBoard))
        // moves.add(moveMapping.get(globalBoard));
        int bestFound = -1;
        int eval;
        // generate Moves
        List<Move> moves = b.getMovesFor(opponent);
        for (int i = moves.size() - 1; i >= 0; i--) {
            // for(int i=0; i<moves.size(); i++){
            // for(Move move: moves){
            Move move = moves.get(i);
            // Attempt to do the move
            b.doMove(move);
            // Check if we have already seen this position
            /*
             * if (evalMapping.containsKey(globalBoard))
             * {
             * eval = evalMapping.get(globalBoard);
             * }
             * else
             * {
             * eval = -Search(depth + 1, -bestFound);
             * evalMapping.put(globalBoard, eval);
             * }
             */
            eval = -score(b, depth + 1, -bestFound);
            if (System.currentTimeMillis() >= timeLimit)
                return 0;
            // Undo move now before returning values
            b.undoMove(move);
            // analyse
            if (eval > bestFound)
                bestFound = eval;
            if (alpha <= bestFound)
                return bestFound;
        }
        return bestFound;
    }

    private int score(Board b, int depth, int alpha) {
        Color winnerColor = b.getWinner(me);
        if (winnerColor == me)
            return 10000;
        else if (winnerColor == opponent)
            return -10000;
        else if (winnerColor == EMPTY)
            return 0;
        // Check if maxDepth was reached
        if (depth == MaxDepth)
            return 0;
        if (System.currentTimeMillis() >= timeLimit)
            return 0;
        // Add best moves from previous iteration
        // if (moveMapping.containsKey(globalBoard))
        // moves.add(moveMapping.get(globalBoard));
        int bestFound = -1;
        int eval;
        List<Move> moves = b.getMovesFor(me);
        for (int i = moves.size() - 1; i >= 0; i--) {
            // for(int i=0; i<moves.size(); i++){
            // for(Move move: moves){
            Move move = moves.get(i);
            // Attempt to do the move
            b.doMove(move);
            // Check if we have already seen this position
            /*
             * if (evalMapping.containsKey(globalBoard))
             * {
             * eval = evalMapping.get(globalBoard);
             * }
             * else
             * {
             * eval = -Search(depth + 1, -bestFound);
             * evalMapping.put(globalBoard, eval);
             * }
             */
            eval = -scoreOpponent(b, depth + 1, -bestFound);
            if (System.currentTimeMillis() >= timeLimit)
                return 0;
            // Undo move now before returning values
            b.undoMove(move);
            // analyse
            if (eval > bestFound)
                bestFound = eval;
            if (alpha <= bestFound)
                return bestFound;
        }
        return bestFound;
    }

    @Override
    public Move nextMove(Board b) {

        timeLimit = System.currentTimeMillis() + getTime() - 50;
        me = getColor();
        opponent = getOpponent(getColor());
        List<Move> moves = b.getMovesFor(me);
        Move selected = moves.get(moves.size() - 1);
        Move bestMove;
        size = b.getSize();
        half = size / 2;
        pivot = half / 2;
        toSide = pivot;
        hoshi[0][0] = pivot;
        hoshi[0][1] = pivot;
        hoshi[1][0] = pivot + half;
        hoshi[1][1] = pivot;
        hoshi[2][0] = pivot;
        hoshi[2][1] = pivot + half;
        hoshi[3][0] = pivot + half;
        hoshi[3][1] = pivot + half;
        opponentBigX = -1;
        opponentSmallX = -1;
        meBigX = -1;
        meSmallX = -1;
        LineX = -1;
        moveAsStr = "";
        int bestFound;
        int eval;
        // Do iterative deepening
        int GlobalMax = 180;
        for (int i = 1; i < 180; i++) {
            // Clear the map before each search
            // evalMapping.clear();
            MaxDepth = i;
            bestMove = moves.get(moves.size() - 1); // for whatever reason it needs to be there to shut down warnings
            bestFound = -1;
            BigThreats(b, opponent, me);
            if (opponentBigX != -1) {
                moveAsStr = StringMove(opponentBigX, opponentBigY);
            } else {
                BigThreats(b, me, opponent);
                if (meBigX != -1) {
                    moveAsStr = StringMove(meBigX, meBigY);
                } else {
                    SmallThreats(b, opponent, me);
                    if (opponentSmallX != -1) {
                        moveAsStr = StringMove(opponentSmallX, opponentSmallY);
                    } else {
                        SmallThreats(b, me, opponent);
                        if (meSmallX != -1) {
                            moveAsStr = StringMove(meSmallX, meSmallY);
                        } else {
                            Line(b);
                        }
                    }
                }
            }
            // add suggested move to the back
            for (int j = moves.size() - 1; j >= 0; j--) {
                Move move = moves.get(j);
                if (move.toString().startsWith(moveAsStr)) {
                    moves.add(move);
                    break;
                }
            }
            // for(Move move: moves) {
            for (int j = moves.size() - 1; j >= 0; j--) {
                Move move = moves.get(j);
                b.doMove(move);
                eval = -scoreOpponent(b, 0, -bestFound);
                if (System.currentTimeMillis() >= timeLimit)
                    return selected;
                b.undoMove(move);
                if (eval > bestFound) {
                    bestFound = eval;
                    bestMove = move;
                }
            }
            selected = bestMove;
        }
        // Probably we will never get there
        return selected;
    }
}
