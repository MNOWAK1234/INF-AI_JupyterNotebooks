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
    public static void main(String[] args) throws IOException {
        Configuration configuration = new Configuration();
        configuration.getCommon().addEventType(KursAkcji.class);
        EPRuntime epRuntime = EPRuntimeProvider.getDefaultRuntime(configuration);
        // Zadanie 0:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select irstream spolka as X, kursOtwarcia as Y
//                from KursAkcji#length(3);""");
        // Zadanie 1:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select irstream spolka as X, kursOtwarcia as Y
//                from KursAkcji#length(3)
//                where spolka = 'Oracle';""");
        // Zadanie 2:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select irstream data, kursOtwarcia, spolka
//                from KursAkcji#length(3)
//                where spolka = 'Oracle';""");
        // Zadanie 3:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select irstream kursOtwarcia, data, spolka
//                from KursAkcji(spolka = 'Oracle')#length(3);""");
        // Zadanie 4:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select istream kursOtwarcia, data, spolka
//                from KursAkcji(spolka = 'Oracle')#length(3);""");
        // Zadanie 5:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select istream max(kursOtwarcia) as kursOtwarcia, data, spolka
//                from KursAkcji(spolka = 'Oracle')
//                #length(5);""");
        // Zadanie 6:
//        EPDeployment deployment = compileAndDeploy(epRuntime,"""
//                select istream (kursOtwarcia - max(kursOtwarcia)) as roznica, data, spolka
//                from KursAkcji(spolka = 'Oracle')
//                #length(5);""");
        // Zadanie 7:
        EPDeployment deployment = compileAndDeploy(epRuntime,"""
                select istream (kursOtwarcia - min(kursOtwarcia)) as roznica, data, spolka
                from KursAkcji(spolka = 'Oracle')
                #length(2)
                having kursOtwarcia > min(kursOtwarcia);""");

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
