package lab_jdbc;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class Exercise5Helper {
    public void executeExercise(Connection conn) {
        String[] nazwiska = {"Woźniak", "Dąbrowski", "Kozłowski"};
        int[] place = {1300, 1700, 1500};
        String[] etaty = {"ASYSTENT", "PROFESOR", "ADIUNKT"};

        try (PreparedStatement pstmt = conn.prepareStatement("insert into pracownicy(id_prac, nazwisko, placa_pod, etat) values (seq_id_prac.nextval, ?, ?, ?)")) {
            for (int i = 0; i < nazwiska.length; i++) {
                pstmt.setString(1, nazwiska[i]);
                pstmt.setInt(2, place[i]);
                pstmt.setString(3, etaty[i]);
                pstmt.executeUpdate();
            }
            System.out.println("Dodano pracowników");

            try (Statement stmt = conn.createStatement(); ResultSet rs = stmt.executeQuery("select nazwa from etaty")) {
                while (rs.next())
                    System.out.println(rs.getString(1));
            } catch (SQLException ex) {
                System.out.println("Error executing query: " + ex.getMessage());
            }

        } catch (SQLException ex) {
            System.out.println("Error executing SQL statement: " + ex.getMessage());
        }
    }
}
