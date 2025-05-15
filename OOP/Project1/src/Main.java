import pl.poznan.put.content.ReadFile;
import pl.poznan.put.content.Team;
import pl.poznan.put.content.ReadInput;
import pl.poznan.put.content.Simulation;

import java.util.Vector;

public class Main {
    public static void main(String args[]){
        Vector<Team> teams = new Vector<>();
        ReadInput input = new ReadInput();
        String coded = input.inputtedSport();
        ReadFile option = new ReadFile();
        String translated = option.translate(coded);
        teams = option.read(translated);
        Simulation season = new Simulation(coded, teams);
        season.simulate();
        season.readResults(teams, coded);
    }
}