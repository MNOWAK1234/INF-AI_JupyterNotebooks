package lab_jdbc;

import java.sql.*;

public class Exercise7Helper {
    public void executeExercise(Connection conn) {
        try (CallableStatement stmt = conn.prepareCall("{ ? = call WielkoscLiter(?, ?) }")) {
            stmt.setInt(2, 160);
            stmt.registerOutParameter(1, Types.INTEGER);
            stmt.registerOutParameter(3, Types.VARCHAR);
            stmt.execute();
            int result = stmt.getInt(1);
            if (result == 0) {
                System.out.println("Nie znaleziono pracownika o podanym id.");
            } else {
                String nazwisko = stmt.getString(3);
                System.out.println(nazwisko);
            }
        } catch (SQLException ex) {
            System.out.println("Błąd wykonania polecenia: " + ex.getMessage());
        }
    }
}
