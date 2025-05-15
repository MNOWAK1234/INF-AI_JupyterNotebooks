package pl.poznan.put.content;

import java.util.Random;

public class Battle {
    Battle() {}
    public static int winner(boolean preference, int ovrFirst, int ovrSecond) {
        Random r = new Random();
        int result = r.nextInt(ovrFirst + ovrSecond) + 1;
        if(preference == true) result -= 100;
        if(result < ovrFirst - 50) return 0;
        else if (result <= ovrFirst + 50) return 1;
        else return 2;
    }
}
