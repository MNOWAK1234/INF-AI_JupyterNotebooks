package lab_jdbc;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.ResultSet;

public class Exercise4Helper {
    public void executeExercise(Connection conn) {
        try {
            conn.setAutoCommit(false);
        } catch (SQLException ex) {
            System.out.println("Error setting auto commit: " + ex.getMessage());
        }

        try (Statement stmt = conn.createStatement()) {
            try (ResultSet rs = stmt.executeQuery("select nazwa from etaty");) {
                while (rs.next())
                    System.out.println(rs.getString(1));
            } catch (SQLException ex) {
                System.out.println("Error executing query 1: " + ex.getMessage());
            }

            int changes = 0;
            try {
                changes = stmt.executeUpdate("insert into etaty(nazwa, placa_min, placa_max) values ('NOWY_ETAT', 100, 200)");
                if (changes == 1) System.out.println("Inserted a new position.");
            } catch (SQLException ex) {
                System.out.println("Error executing insert query: " + ex.getMessage());
            }

            try (ResultSet rs = stmt.executeQuery("select nazwa from etaty");) {
                while (rs.next())
                    System.out.println(rs.getString(1));
            } catch (SQLException ex) {
                System.out.println("Error executing query 2: " + ex.getMessage());
            }

            try {
                conn.rollback();
                System.out.println("Rolled back the transaction");

                try (ResultSet rs = stmt.executeQuery("select nazwa from etaty");) {
                    while (rs.next())
                        System.out.println(rs.getString(1));
                } catch (SQLException ex) {
                    System.out.println("Error executing query 3: " + ex.getMessage());
                }

                changes = stmt.executeUpdate("insert into etaty(nazwa, placa_min, placa_max) values ('NOWY_ETAT', 10, 20)");
                if (changes == 1) System.out.println("Inserted a new position.");

                conn.commit();
                System.out.println("Committed the transaction");

                try (ResultSet rs = stmt.executeQuery("select nazwa from etaty");) {
                    while (rs.next())
                        System.out.println(rs.getString(1));
                } catch (SQLException ex) {
                    System.out.println("Error executing query 4: " + ex.getMessage());
                }

            } catch (SQLException ex) {
                System.out.println("Error during rollback/commit: " + ex.getMessage());
            }
        } catch (SQLException ex) {
            System.out.println("Error executing SQL statements: " + ex.getMessage());
        }
    }
}
