package lab_jdbc;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class Exercise6Helper {
    public void executeExercise(Connection conn) {
        try {
            conn.setAutoCommit(false);
        } catch (SQLException ex) {
            System.out.println("Error setting auto commit: " + ex.getMessage());
        }

        try (PreparedStatement pstmt = conn.prepareStatement("insert into pracownicy(id_prac, nazwisko) values (seq_id_prac.nextval, 'Kowalski')")) {
            long start = System.nanoTime();
            for (int i = 0; i < 2000; i++)
                pstmt.executeUpdate();
            long czas = System.nanoTime() - start;
            System.out.println("(1) took " + czas / 1000000 + "ms time");

            start = System.nanoTime();
            for (int i = 0; i < 2000; i++)
                pstmt.addBatch();
            pstmt.executeBatch();
            czas = System.nanoTime() - start;
            System.out.println("(2) took " + czas / 1000000 + "ms time");

            conn.commit();
        } catch (SQLException ex) {
            System.out.println("Error executing SQL statement: " + ex.getMessage());
        }
    }
}
