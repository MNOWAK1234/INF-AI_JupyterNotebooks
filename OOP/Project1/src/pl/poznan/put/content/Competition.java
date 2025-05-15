package pl.poznan.put.content;

import java.util.Vector;

public class Competition implements ICompetition{
    public Vector<Team> roadToGlory(Vector<Team> teams, Vector<Integer> scoringSheet){
        Vector<Boolean>eliminated = new Vector<>();
        for( int i = 0; i < teams.size(); i++) {
            eliminated.add(false);
        }
        int front = 0;
        int back = 31;
        for(int i = 0; i < 5; i++) {
            front = 0;
            back = 31;
            while(front < back) {
                if(eliminated.get(front) == false && eliminated.get(back) == false) {
                    if(Battle.winner(true, teams.get(front).strength(), teams.get(back).strength()) < 2) {
                        eliminated.set(back, true);
                        teams.get(front).achievePoints(1);
                        switch (i) {
                            case (0):
                            teams.get(back).result("Eliminated 1/16");
                            break;
                            case (1):
                            teams.get(back).result("Eliminated 1/8");
                            break;
                            case (2):
                            teams.get(back).result("Eliminated 1/4");
                            break;
                            case (3):
                            teams.get(back).result("Eliminated 1/2");
                            break;
                            case (4):
                            teams.get(back).result("2ND");
                            teams.get(front).result("1ST");
                            break;
                        }
                    }
                    else {
                        eliminated.set(front, true);
                        teams.get(back).achievePoints(1);
                        switch (i) {
                            case (0):
                            teams.get(front).result("Eliminated 1/16");
                            break;
                            case (1):
                            teams.get(front).result("Eliminated 1/8");
                            break;
                            case (2):
                            teams.get(front).result("Eliminated 1/4");
                            break;
                            case (3):
                            teams.get(front).result("Eliminated 1/2");
                            break;
                            case (4):
                            teams.get(front).result("2ND");
                            teams.get(back).result("1ST");
                            break;
                        }
                    }
                    front++;
                    back--;
                }
                else if (eliminated.get(front) == true) {
                    front++;
                }
                else if (eliminated.get(back) == true) {
                    back--;
                }
            }
        }
        return teams;
    }
}
