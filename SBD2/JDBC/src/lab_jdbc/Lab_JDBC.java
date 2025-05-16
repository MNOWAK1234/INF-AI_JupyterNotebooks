package lab_jdbc;

import java.util.Scanner;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;

public class Lab_JDBC {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Enter the exercise number (1 to 7): ");
        int exerciseNumber = scanner.nextInt();

        Connection conn = null;
        String connectionString = "jdbc:oracle:thin:@admlab2.cs.put.poznan.pl:1521/dblab03_students.cs.put.poznan.pl";
        Properties connectionProps = new Properties();
        connectionProps.put("user", "sbd151813");
        connectionProps.put("password", "sbd151813");

        try {
            conn = DriverManager.getConnection(connectionString, connectionProps);
            System.out.println("Connected to the database");

            switch (exerciseNumber) {
                case 1:
                    Exercise1Helper exercise1 = new Exercise1Helper();
                    exercise1.executeExercise(conn);
                    break;
                case 2:
                    Exercise2Helper exercise2 = new Exercise2Helper();
                    exercise2.executeExercise(conn);
                    break;
                case 3:
                    Exercise3Helper exercise3 = new Exercise3Helper();
                    exercise3.executeExercise(conn);
                    break;
                case 4:
                    Exercise4Helper exercise4 = new Exercise4Helper();
                    exercise4.executeExercise(conn);
                    break;
                case 5:
                    Exercise5Helper exercise5 = new Exercise5Helper();
                    exercise5.executeExercise(conn);
                    break;
                case 6:
                    //(1) took 33052ms time
                    //(2) took 60ms time
                    Exercise6Helper exercise6 = new Exercise6Helper();
                    exercise6.executeExercise(conn);
                    break;
                case 7:
                    /*CREATE OR REPLACE FUNCTION WielkoscLiter (
                        pid_prac IN Pracownicy.id_prac%TYPE,
                        zmienione_nazwisko OUT Pracownicy.nazwisko%TYPE)
                        RETURN NUMBER IS
                        wynik NUMBER;
                        vnazwisko Pracownicy.nazwisko%TYPE;
                        BEGIN
                        SELECT (SELECT nazwisko FROM pracownicy WHERE id_prac=pid_prac) INTO vnazwisko FROM DUAL;
                        wynik := 1;
                        IF vnazwisko IS NULL THEN
                        wynik := 0;
                        ELSE
                        zmienione_nazwisko := initcap(vnazwisko);
                        END IF;
                        RETURN wynik;
                        END;*/
                    Exercise7Helper exercise7 = new Exercise7Helper();
                    exercise7.executeExercise(conn);
                    break;
                default:
                    System.out.println("Invalid exercise number");
            }
        } catch (SQLException ex) {
            System.out.println("Error: " + ex.getMessage());
        } finally {
            try {
                if (conn != null) {
                    conn.close();
                    System.out.println("Disconnected from the database");
                }
            } catch (SQLException ex) {
                System.out.println("Error while closing connection: " + ex.getMessage());
            }
            scanner.close();
        }
    }
}
