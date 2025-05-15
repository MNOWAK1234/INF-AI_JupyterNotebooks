package pl.poznan.put.content;

import java.util.Vector;
import java.util.Random;
import java.util.Collections;

public class Formula1 implements IFormula1 {
    public Vector<Team> wholeSeason(Vector<Team> teams, Vector<Integer> scoringSheet) {
        F1season season = new F1season();
        Vector<Integer>races = new Vector<>();
        races = season.publishCalendar();
        Vector<RaceResults>grandPrix = new Vector<>();
        int perf = 0;
        for(int i = 0; i < races.size(); i++) {
            for(int j = 0; j < teams.size(); j++) {
                perf = 0;
                Random r = new Random();
                for(int k = 0; k < races.get(i); k++) {
                    perf += r.nextInt(teams.get(j).strength()) + 1;
                }
                grandPrix.add(new RaceResults(j, perf));
            }
            Collections.sort(grandPrix, (d1, d2) -> d2.pace() - d1.pace());
            for(int j = 0; j < teams.size(); j++) {
                teams.get(grandPrix.get(j).racingNumber()).achievePoints(scoringSheet.get(j));
            }
            grandPrix.clear();
        }
        return teams;
    }
}
