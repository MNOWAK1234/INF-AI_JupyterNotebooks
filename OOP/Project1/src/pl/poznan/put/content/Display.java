package pl.poznan.put.content;

import java.util.Vector;

public class Display implements IDisplay{
    public void showTeam(Team one) {
        System.out.println(one.name + " " + one.points + " " + one.ovr);
    }
    public void readResults(Vector<Team> teams, String sport) {
        if(sport.equals("Football")) {
            for(int i = 0; i < teams.size(); i++) {
                System.out.println(i+1 + ". " + teams.get(i).tellName() + " " + teams.get(i).tally());
            }
        }
        else if(sport.equals("F1")) {
            for(int i = 0; i < teams.size(); i++) {
                System.out.println(i+1 + ". " + teams.get(i).tellName() + " " + teams.get(i).tally());
            }
        }
        else if(sport.equals("Cup")) {
            for(int i = 0; i < teams.size(); i++) {
                System.out.println(teams.get(i).stage() + ": " + teams.get(i).tellName());
            }
        }
    }    
}
