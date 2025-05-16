package lab_jdbc;

import java.sql.Connection;
import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class Exercise1Helper {
    public void executeExercise(Connection conn) {
        try (Statement stmt = conn.createStatement()) {
            try (ResultSet rs1 = stmt.executeQuery("select count(id_prac) from pracownicy")) {
                rs1.next();
                System.out.println("Zatrudniono " + rs1.getInt(1) + " pracowników, w tym:");
            } catch (SQLException ex) {
                System.out.println("Błąd wykonania polecenia 1: " + ex.getMessage());
            }
            try (ResultSet rs2 = stmt.executeQuery("select count(id_prac), nazwa from pracownicy natural inner join zespoly group by nazwa")) {
                while (rs2.next()) {
                    System.out.println(rs2.getInt(1) + " w zespole " + rs2.getString(2) + ",");
                }
            } catch (SQLException ex) {
                System.out.println("Błąd wykonania polecenia 2: " + ex.getMessage());
            }
        } catch (SQLException ex) {
            System.out.println("Błąd wykonania polecenia ogólny: " + ex.getMessage());
        }
    }
}
