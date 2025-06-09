package com.example.bigdata;

import com.espertech.esper.common.client.EPCompiled;
import com.espertech.esper.common.client.EventBean;
import com.espertech.esper.common.client.configuration.Configuration;
import com.espertech.esper.compiler.client.CompilerArguments;
import com.espertech.esper.compiler.client.EPCompileException;
import com.espertech.esper.compiler.client.EPCompiler;
import com.espertech.esper.compiler.client.EPCompilerProvider;
import com.espertech.esper.runtime.client.*;

public class EsperClient {

    public static void main(String[] args) {

        Configuration config = new Configuration();
        EPCompiled epCompiled = getEPCompiled(config);

        // Connect to the EPRuntime server and deploy the statement
        EPRuntime runtime = EPRuntimeProvider.getRuntime("http://localhost:port", config);
        EPDeployment deployment;
        try {
            deployment = runtime.getDeploymentService().deploy(epCompiled);
        }
        catch (EPDeployException ex) {
            // handle exception here
            throw new RuntimeException(ex);
        }

        EPStatement resultStatement = runtime.getDeploymentService().getStatement(deployment.getDeploymentId(), "answer");


        // Add a listener to the statement to handle incoming events
        // String processing and making JsonObject out of it
        resultStatement.addListener( (newData, oldData, stmt, runTime) -> {
            for (EventBean eventBean : newData) {
                System.out.println(eventBean.getUnderlying());
            }
        });

        for (String s : createInputData()) {
            runtime.getEventService().sendEventJson(s, "Ticker");
        }
    }

    private static EPCompiled getEPCompiled(Configuration config) {
        CompilerArguments compilerArgs = new CompilerArguments(config);

        // Compile the EPL statement
        EPCompiler compiler = EPCompilerProvider.getCompiler();
        EPCompiled epCompiled;
        try {
            // Zadanie 1:
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema Ticker(symbol string, tstamp string, price int);
//                    create window AcmeTicker#length(10) as Ticker;
//                    insert into AcmeTicker select * from Ticker where symbol = 'ACME';
//                    @name('answer') select a.* from pattern[ every a=AcmeTicker(price=25) ];
//                    """, compilerArgs);
            // Zadanie 2:
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema Ticker(symbol string, tstamp string, price int);
//                    create window AcmeTicker#length(10) as Ticker;
//                    insert into AcmeTicker select * from Ticker where symbol = 'ACME';
//                    @name('answer') select a.tstamp, b.price - a.price as diff, b.tstamp
//                    from pattern[ every a=AcmeTicker -> b=AcmeTicker ];
//                    """, compilerArgs);
            // Zadanie 3:
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema Ticker(symbol string, tstamp string, price int);
//                    create window AcmeTicker#length(10) as Ticker;
//                    insert into AcmeTicker select * from Ticker where symbol = 'ACME';
//                    @name('answer') select a.tstamp, b.price - a.price as diff, b.tstamp
//                    from pattern[ every a=AcmeTicker -> b=AcmeTicker ]
//                    where b.price < a.price;
//                    """, compilerArgs);
            // Zadanie 4:
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema Ticker(symbol string, tstamp string, price int);
//                    create window AcmeTicker#length(10) as Ticker;
//                    insert into AcmeTicker select * from Ticker where symbol = 'ACME';
//                    @name('answer') select a.tstamp, b.price, b.tstamp, a.price
//                    from pattern[ every a=AcmeTicker(price>23) -> b=AcmeTicker(price>23) ];
//                    """, compilerArgs);
            // Zadanie 5:
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema Ticker(symbol string, tstamp string, price int);
//                    create window AcmeTicker#length(10) as Ticker;
//                    insert into AcmeTicker select * from Ticker where symbol = 'ACME';
//                    @name('answer') select a.tstamp, b.price, b.tstamp, a.price
//                    from pattern[ every a=AcmeTicker(price>23) -> (b=AcmeTicker(price>23)
//                    and not AcmeTicker(price<14)) ];
//                    """, compilerArgs);
            // Zadanie 6:
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema Ticker(symbol string, tstamp string, price int);
//                    create window AcmeTicker#length(10) as Ticker;
//                    insert into AcmeTicker select * from Ticker where symbol = 'ACME';
//                    @name('answer') select a[0].price as p1, a[0].tstamp as tstart, a[1].price as p2, a[2].price as p3
//                    from pattern[ every ([3:] a=AcmeTicker(price>=20) until AcmeTicker(price<20)) ];
//                    """, compilerArgs);
            // Zadanie 7:
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema Ticker(symbol string, tstamp string, price int);
//                    create window AcmeTicker#length(100) as Ticker;
//                    insert into AcmeTicker select * from Ticker where symbol = 'ACME';
//                    @name('answer')
//                    select *
//                    from AcmeTicker
//                    match_recognize (
//                       partition by symbol
//                       measures strt.symbol as symbol,
//                                strt.tstamp as startT,
//                                strt.price as startPrice,
//                                last(down.price) as lowPrice,
//                                last(up.price) as highPrice,
//                                last(up.tstamp) as stopT
//                       pattern (strt down+ up+)
//                       define
//                           down as down.price < prev(down.price),
//                           up as up.price > prev(up.price)
//                    );
//                    """, compilerArgs);
            // Zadanie 8:
            epCompiled = compiler.compile("""
                    @public @buseventtype create json schema Ticker(symbol string, tstamp string, price int);
                    create window AcmeTicker#length(100) as Ticker;
                    @name('answer')
                    select *
                    from Ticker#length(100)
                    match_recognize (
                         partition by symbol
                         measures strt.symbol as symbol,
                                  strt.tstamp as startT,
                                  strt.price as startPrice,
                                  last(down.price) as lowPrice,
                                  last(up.price) as highPrice,
                                  last(up.tstamp) as stopT
                          pattern (strt down+ up+)
                          define
                              down as down.price < prev(down.price),
                              up as up.price > prev(up.price)
                    );
                    """, compilerArgs);
            // Zadania 9 i 10 nie są obowiązkowe:
        }
        catch (EPCompileException ex) {
            // handle exception here
            throw new RuntimeException(ex);
        }
        return epCompiled;
    }

    static String[] createInputData() {
        return new String[] {
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-01 00:00:00.0\", \"price\":12}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-01 00:00:00.0\", \"price\":11}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-01 00:00:00.0\", \"price\":22}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-02 00:00:00.0\", \"price\":17}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-02 00:00:00.0\", \"price\":12}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-02 00:00:00.0\", \"price\":22}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-03 00:00:00.0\", \"price\":19}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-03 00:00:00.0\", \"price\":13}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-03 00:00:00.0\", \"price\":19}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-04 00:00:00.0\", \"price\":21}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-04 00:00:00.0\", \"price\":12}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-04 00:00:00.0\", \"price\":18}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-05 00:00:00.0\", \"price\":25}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-05 00:00:00.0\", \"price\":11}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-05 00:00:00.0\", \"price\":17}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-06 00:00:00.0\", \"price\":12}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-06 00:00:00.0\", \"price\":10}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-06 00:00:00.0\", \"price\":20}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-07 00:00:00.0\", \"price\":15}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-07 00:00:00.0\", \"price\":9}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-07 00:00:00.0\", \"price\":17}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-08 00:00:00.0\", \"price\":20}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-08 00:00:00.0\", \"price\":8}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-08 00:00:00.0\", \"price\":20}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-09 00:00:00.0\", \"price\":24}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-09 00:00:00.0\", \"price\":9}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-09 00:00:00.0\", \"price\":16}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-10 00:00:00.0\", \"price\":25}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-10 00:00:00.0\", \"price\":9}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-10 00:00:00.0\", \"price\":15}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-11 00:00:00.0\", \"price\":19}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-11 00:00:00.0\", \"price\":9}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-11 00:00:00.0\", \"price\":15}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-12 00:00:00.0\", \"price\":15}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-12 00:00:00.0\", \"price\":9}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-12 00:00:00.0\", \"price\":12}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-13 00:00:00.0\", \"price\":25}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-13 00:00:00.0\", \"price\":10}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-13 00:00:00.0\", \"price\":11}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-14 00:00:00.0\", \"price\":25}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-14 00:00:00.0\", \"price\":11}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-14 00:00:00.0\", \"price\":15}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-15 00:00:00.0\", \"price\":14}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-15 00:00:00.0\", \"price\":12}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-15 00:00:00.0\", \"price\":12}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-16 00:00:00.0\", \"price\":12}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-16 00:00:00.0\", \"price\":11}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-16 00:00:00.0\", \"price\":16}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-17 00:00:00.0\", \"price\":14}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-17 00:00:00.0\", \"price\":8}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-17 00:00:00.0\", \"price\":14}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-18 00:00:00.0\", \"price\":24}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-18 00:00:00.0\", \"price\":7}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-18 00:00:00.0\", \"price\":12}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-19 00:00:00.0\", \"price\":23}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-19 00:00:00.0\", \"price\":5}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-19 00:00:00.0\", \"price\":11}",
                "{\"symbol\":\"ACME\", \"tstamp\":\"2011-04-20 00:00:00.0\", \"price\":22}",
                "{\"symbol\":\"GLOBEX\", \"tstamp\":\"2011-04-20 00:00:00.0\", \"price\":3}",
                "{\"symbol\":\"OSCORP\", \"tstamp\":\"2011-04-20 00:00:00.0\", \"price\":9}"
        };
    }
}

