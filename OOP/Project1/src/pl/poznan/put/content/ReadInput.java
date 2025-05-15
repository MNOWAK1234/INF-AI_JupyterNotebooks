package pl.poznan.put.content;

import java.util.Scanner;

public class ReadInput {
  public String inputtedSport() {
    Scanner scanner = new Scanner(System.in);
    System.out.println("Enter type of competition:");
    String type = scanner.nextLine();
    scanner.close(); 
    return type;
  }
}
