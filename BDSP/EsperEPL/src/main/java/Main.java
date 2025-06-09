import java.io.IOException;
import com.espertech.esper.common.client.configuration.Configuration;
import com.espertech.esper.runtime.client.EPRuntime;
import com.espertech.esper.runtime.client.EPRuntimeProvider;
import com.espertech.esper.common.client.EPCompiled;
import com.espertech.esper.compiler.client.CompilerArguments;
import com.espertech.esper.compiler.client.EPCompileException;
import com.espertech.esper.compiler.client.EPCompilerProvider;
import com.espertech.esper.runtime.client.*;

public class Main {

    //Zadanie 3b:
    public static boolean czyWzrost(double kursZamkniecia, double kursOtwarcia) {
        return kursZamkniecia > kursOtwarcia;
    }

    public static void main(String[] args) throws IOException {
        Configuration configuration = new Configuration();
        configuration.getCommon().addEventType(KursAkcji.class);
        EPRuntime epRuntime = EPRuntimeProvider.getDefaultRuntime(configuration);
        // Zadanie 1:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select istream data, kursZamkniecia, spolka, max(kursZamkniecia) - kursZamkniecia as roznica
//                from KursAkcji#ext_timed_batch(data.getTime(), 1 days);""");
        // Zadanie 2:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select istream data, kursZamkniecia, spolka, max(kursZamkniecia) - kursZamkniecia as roznica
//                from KursAkcji(spolka in ['IBM', 'Honda', 'Microsoft'])
//                #ext_timed_batch(data.getTime(), 1 days);""");
        // Zadanie 3a:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select istream data, kursZamkniecia, kursOtwarcia, spolka
//                from KursAkcji(kursZamkniecia > kursOtwarcia)
//                #length(1);""");
        // Zadanie 3b: Korzystając z warunku opartego na metodzie statycznej
//        EPDeployment deployment = compileAndDeploy(epRuntime, """
//                select istream data, kursZamkniecia, kursOtwarcia, spolka
//                from KursAkcji(KursAkcji.czyWzrost(kursZamkniecia, kursOtwarcia))
//                #length(1);""");
        // Zadanie 4:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select istream data, kursZamkniecia, spolka, max(kursZamkniecia) - kursZamkniecia as roznica
//                from KursAkcji(spolka in ['PepsiCo', 'CocaCola'])
//                #ext_timed(data.getTime(), 7 days);""");
        // Zadanie 5:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select istream data, kursZamkniecia, spolka
//                from KursAkcji(spolka in ['PepsiCo', 'CocaCola'])
//                #ext_timed_batch(data.getTime(), 1 days)
//                having kursZamkniecia = max(kursZamkniecia);""");
        // Zadanie 6:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select istream max(kursZamkniecia) as maksimum
//                from KursAkcji
//                #ext_timed_batch(data.getTime(), 7 days);""");
        // Zadanie 7:
//        EPDeployment deployment = compileAndDeploy(epRuntime, """
//                select c.kursZamkniecia as kursCoc, p.data, p.kursZamkniecia as kursPep
//                from KursAkcji(spolka='CocaCola')#length(1) as c
//                full outer join KursAkcji(spolka='PepsiCo')#length(1) as p
//                on   c.data = p.data
//                where c.kursZamkniecia < p.kursZamkniecia;""");
        // Zadanie 8:
//        EPDeployment deployment = compileAndDeploy(epRuntime, """
//                select istream k.data, k.kursZamkniecia as kursBiezacy, k.spolka, k.kursZamkniecia - f.kursZamkniecia as roznica
//                from KursAkcji(spolka in ('CocaCola', 'PepsiCo'))#length(1) as k
//                inner join KursAkcji()#firstunique(spolka) as f on f.spolka = k.spolka;""");
        // Zadanie 9:
//        EPDeployment deployment = compileAndDeploy(epRuntime, """
//                select istream k.data, k.kursZamkniecia as kursBiezacy, k.spolka, k.kursZamkniecia - f.kursZamkniecia as roznica
//                from KursAkcji()#length(1) as k
//                inner join KursAkcji()#firstunique(spolka) as f on f.spolka = k.spolka
//                where k.kursZamkniecia > f.kursZamkniecia;""");
        // Zadanie 10:
//        EPDeployment deployment = compileAndDeploy(epRuntime, """
//                select istream k.data as dataB, a.data as dataA,
//                k.spolka, a.kursOtwarcia as kursA, k.kursOtwarcia as kursB
//                from KursAkcji()#length(1) as k
//                inner join KursAkcji()#ext_timed(data.getTime(), 7 days) as a
//                on a.spolka = k.spolka
//                having a.kursOtwarcia - k.kursOtwarcia > 3 or k.kursOtwarcia - a.kursOtwarcia > 3;""");
        // Zadanie 11:
//        EPDeployment deployment = compileAndDeploy(epRuntime, """
//                select istream data, spolka, obrot
//                from KursAkcji(market = 'NYSE')#ext_timed_batch(data.getTime(), 7 days)
//                order by obrot desc
//                limit 3;""");
        // Zadanie 12:
        EPDeployment deployment = compileAndDeploy(epRuntime, """
                select istream data, spolka, obrot
                from KursAkcji(market = 'NYSE')#ext_timed_batch(data.getTime(), 7 days)
                order by obrot desc
                limit 1 offset 2;""");

        ProstyListener prostyListener = new ProstyListener();
        for (EPStatement statement : deployment.getStatements()) {
            statement.addListener(prostyListener);
        }
        CreateInputStream inputStream = new CreateInputStream();
        inputStream.generuj(epRuntime.getEventService());
    }

    public static EPDeployment compileAndDeploy(EPRuntime epRuntime, String epl) {
        EPDeploymentService deploymentService = epRuntime.getDeploymentService();

        CompilerArguments args =
                new CompilerArguments(epRuntime.getConfigurationDeepCopy());
        EPDeployment deployment;
        try {
            EPCompiled epCompiled = EPCompilerProvider.getCompiler().compile(epl, args);
            deployment = deploymentService.deploy(epCompiled);
        } catch (EPCompileException | EPDeployException e) {
            throw new RuntimeException(e);
        }

        return deployment;
    }
}
