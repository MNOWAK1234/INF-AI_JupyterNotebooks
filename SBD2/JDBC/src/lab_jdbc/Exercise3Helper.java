package lab_jdbc;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

public class Exercise3Helper {
    public void executeExercise(Connection conn) {
        int[] zwolnienia = {150, 200, 230};
        String[] zatrudnienia = {"Kandefer", "Rygiel", "Boczar"};

        try (Statement stmt = conn.createStatement()) {
            int changes = 0;
            for (int id_prac : zwolnienia)
                changes += stmt.executeUpdate("delete from pracownicy where id_prac=" + id_prac);
            System.out.println("Usunieto " + changes + " krotek.");

            changes = 0;
            for (String nazwisko : zatrudnienia)
                changes += stmt.executeUpdate("insert into pracownicy(id_prac, nazwisko) values (seq_id_prac.nextval, '" + nazwisko + "')");
            System.out.println("Wstawiono " + changes + " krotek.");
        } catch (SQLException ex) {
            System.out.println("Błąd wykonania polecenia: " + ex.getMessage());
        }
    }
}
