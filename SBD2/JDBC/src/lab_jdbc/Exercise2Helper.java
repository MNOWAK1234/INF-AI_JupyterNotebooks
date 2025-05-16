package lab_jdbc;

import java.sql.Connection;
import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class Exercise2Helper {
    public void executeExercise(Connection conn) {
        try (Statement stmt = conn.createStatement(ResultSet.TYPE_SCROLL_SENSITIVE, ResultSet.CONCUR_READ_ONLY)) {
            try (ResultSet rs = stmt.executeQuery("select id_prac, nazwisko from pracownicy where etat='ASYSTENT' order by placa_pod desc")) {
                rs.next();
                System.out.println(rs.getInt(1) + " " + rs.getString(2));
                rs.relative(2);
                System.out.println(rs.getInt(1) + " " + rs.getString(2));
                rs.absolute(-2);
                System.out.println(rs.getInt(1) + " " + rs.getString(2));
            } catch (SQLException ex) {
                System.out.println("Błąd wykonania polecenia: " + ex.getMessage());
            }
        } catch (SQLException ex) {
            System.out.println("Błąd wykonania polecenia ogólny: " + ex.getMessage());
        }
    }
}
