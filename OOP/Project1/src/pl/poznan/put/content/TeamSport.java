package pl.poznan.put.content;

import java.util.Vector;

public class TeamSport implements ITeamSport {
    public Vector<Team> wholeLeague(Vector<Team> teams, Vector<Integer> scoringSheet) {
        FootballLeague PremierLeague = new FootballLeague("League", teams);
        Vector<Integer>pairing = new Vector<>();
        pairing = PremierLeague.generatePairing();
        if(pairing.get(0) == -2) {
            for(int i = 1; i < pairing.size(); i += 2) {
                int result = Battle.winner(true, teams.get(pairing.get(i)).strength(), teams.get(pairing.get(i+1)).strength());
                teams.get(pairing.get(i)).achievePoints(scoringSheet.get(result));
                if(result == 2) result = 0;
                else if (result == 0) result = 2;
                teams.get(pairing.get(i+1)).achievePoints(scoringSheet.get(result));
            }
        }
        return teams;
    }
}
