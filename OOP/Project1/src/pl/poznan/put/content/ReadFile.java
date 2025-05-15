package pl.poznan.put.content;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.Vector;
import java.util.Random;

public class ReadFile {
  public String translate (String competition) {
    switch(competition) {
      case("Football"):
        return "PremierLeague.txt";
      case("F1"):
        return "Formula1.txt";
      case("Cup"):
        return "WorldCup.txt";
      default: return "Mistake.txt";
    }
  }
  public Vector<Team> read(String competition) {
    Vector<Team> teams = new Vector<Team>();
    try {
      File myObj = new File("src/pl/poznan/put/ExampleData/"+competition);
      Scanner myReader = new Scanner(myObj);
      while (myReader.hasNextLine()) {
        String data = myReader.nextLine();
        int i = data.length()-1;
        while(data.charAt(i) != ' ') {
            i-=1;
        }
        int ovr = 0;
        String name = data.substring(0,i);
        String ovrUnprocessed = data.substring(i+1,data.length());
        try{
          ovr = Integer.parseInt(ovrUnprocessed);
        } catch (NumberFormatException e) {
          System.out.println("Error: " + ovrUnprocessed + " is not a valid integer.");
          Random r = new Random();
          ovr = r.nextInt(1000)+1;
      }catch(ArithmeticException e){
          System.out.println("Error: " + ovrUnprocessed + " is not a valid number.");
          Random r = new Random();
          ovr = r.nextInt(1000)+1;
      }finally{
        teams.add(new Team(name, ovr));
      }
      }
      myReader.close();
      return teams;
    } catch (FileNotFoundException e) {
      System.out.println("An error occurred.");
      e.printStackTrace();
    }
    return teams;
  }
  public ReadFile() {}
}
