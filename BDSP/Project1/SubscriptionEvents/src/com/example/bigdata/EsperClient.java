package com.example.bigdata;

import com.espertech.esper.common.client.EPCompiled;
import com.espertech.esper.common.client.EventBean;
import com.espertech.esper.common.client.configuration.Configuration;
import com.espertech.esper.compiler.client.CompilerArguments;
import com.espertech.esper.compiler.client.EPCompileException;
import com.espertech.esper.compiler.client.EPCompiler;
import com.espertech.esper.compiler.client.EPCompilerProvider;
import com.espertech.esper.runtime.client.*;
import net.datafaker.Faker;
import net.datafaker.fileformats.Format;

import java.sql.Timestamp;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.time.temporal.ChronoUnit;
import java.util.Random;
import java.util.concurrent.TimeUnit;

public class EsperClient {
    public static void main(String[] args) throws InterruptedException {
        int noOfRecordsPerSec;
        int howLongInSec;
        if (args.length < 2) {
            noOfRecordsPerSec = 20;
            howLongInSec = 60;
        } else {
            noOfRecordsPerSec = Integer.parseInt(args[0]);
            howLongInSec = Integer.parseInt(args[1]);
        }

        Configuration config = new Configuration();
        EPCompiled epCompiled = getEPCompiled(config);

        // Connect to the EPRuntime server and deploy the statement
        EPRuntime runtime = EPRuntimeProvider.getRuntime("http://localhost:port", config);
        EPDeployment deployment;
        try {
            deployment = runtime.getDeploymentService().deploy(epCompiled);
        } catch (EPDeployException ex) {
            // handle exception here
            throw new RuntimeException(ex);
        }

        EPStatement resultStatement = runtime.getDeploymentService().getStatement(deployment.getDeploymentId(), "answer");

        // Add a listener to the statement to handle incoming events
        resultStatement.addListener((newData, oldData, stmt, runTime) -> {
            for (EventBean eventBean : newData) {
                System.out.printf("R: %s%n", eventBean.getUnderlying());
            }
        });

        Random generator = new Random(1);
        Service.setRandom(generator);
        Faker faker = new Faker(generator);
        String record;

        long startTime = System.currentTimeMillis();
        while (System.currentTimeMillis() < startTime + (1000L * howLongInSec)) {
            for (int i = 0; i < noOfRecordsPerSec; i++) {
                String gender = faker.gender().types();
                String paymentMethod = faker.subscription().paymentMethods();
                String plan = faker.subscription().plans().replace("Free Trial", "Starter");

                Timestamp timestamp = faker.date().past(30, TimeUnit.SECONDS);
                Instant eventTimestamp = timestamp.toInstant().truncatedTo(ChronoUnit.SECONDS);
                String ets = LocalDateTime.ofInstant(eventTimestamp, ZoneId.systemDefault()).format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"));

                Instant now = Instant.now().truncatedTo(ChronoUnit.SECONDS);
                String its = LocalDateTime.ofInstant(now, ZoneId.systemDefault()).format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"));
                record = Format.toJson()
                        .set("gender", () -> gender)
                        .set("age", () -> faker.number().numberBetween(18, 101))
                        .set("service", Service::randomService)
                        .set("paymentMethod", () -> paymentMethod)
                        .set("plan", () -> plan)
                        .set("price", () -> faker.number().numberBetween(40, 101))
                        .set("ets", () -> ets)
                        .set("its", () -> its)
                        .build().generate();
                runtime.getEventService().sendEventJson(record, "SubscriptionEvent");
            }
            waitToEpoch();
        }
    }

    private static EPCompiled getEPCompiled(Configuration config) {
        CompilerArguments compilerArgs = new CompilerArguments(config);

        // Compile the EPL statement
        EPCompiler compiler = EPCompilerProvider.getCompiler();
        EPCompiled epCompiled;
        try {
            // Zadanie 1
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema SubscriptionEvent(gender string, age int, service string, paymentMethod string, plan string, price int, ets string, its string);
//                    @name('answer')
//                    SELECT gender, AVG(price) AS avg_price
//                    FROM SubscriptionEvent#ext_timed(java.sql.Timestamp.valueOf(its).getTime(), 30 sec)
//                    GROUP BY gender
//                    """, compilerArgs);
            // Zadanie 2
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema SubscriptionEvent(gender string, age int, service string, paymentMethod string, plan string, price int, ets string, its string);
//                    @name('answer')
//                    select gender, age, service, ets, its
//                    from SubscriptionEvent
//                    where gender = 'Female' and age = 100;
//                    """, compilerArgs);
            // Zadanie 3
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema SubscriptionEvent(gender string, age int, service string, paymentMethod string, plan string, price int, ets string, its string);
//                    @name('answer')
//                    select
//                        a.plan as plan,
//                        avg(b.age) as avg_age,
//                        avg(b.price) as avg_price,
//                        a.age as age,
//                        a.price as price,
//                        a.ets as ets,
//                        a.its as its
//                    from SubscriptionEvent#length(1) a,
//                        SubscriptionEvent#ext_timed(java.sql.Timestamp.valueOf(its).getTime(), 30 sec) b
//                    where a.plan = b.plan
//                    group by a.plan, a.age, a.price, a.ets, a.its
//                    having a.age >= avg(b.age) + 10
//                    """, compilerArgs);
            // Zadanie 4
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema SubscriptionEvent(gender string, age int, service string, paymentMethod string, plan string, price int, ets string, its string);
//                    @name('answer')
//                    SELECT
//                        service,
//                        SUM(CASE WHEN age > 60 THEN price ELSE 0 END) AS old_price_sum,
//                        SUM(CASE WHEN age <= 60 THEN price ELSE 0 END) AS young_price_sum
//                    FROM
//                        SubscriptionEvent#ext_timed(java.sql.Timestamp.valueOf(its).getTime(), 1 min)
//                    GROUP BY
//                        service;
//                    """, compilerArgs);
            // Zadanie 5
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema SubscriptionEvent(gender string, age int, service string, paymentMethod string, plan string, price int, ets string, its string);
//                    @name('answer')
//                    SELECT
//                        first(age) AS age,
//                        first(plan) AS plan,
//                        first(service) AS service,
//                        first(ets) AS ets,
//                        first(its) AS its
//                    FROM SubscriptionEvent(
//                        age <= 26 AND plan = 'Student'
//                    )#ext_timed_batch(java.sql.Timestamp.valueOf(its).getTime(), 60 sec)
//                    HAVING
//                        (java.sql.Timestamp.valueOf(first(its)).getTime() -
//                        min(java.sql.Timestamp.valueOf(its).getTime())) <= 5000
//                    """, compilerArgs);
            // Zadanie 6
//            epCompiled = compiler.compile("""
//                    @public @buseventtype create json schema SubscriptionEvent(gender string, age int, service string, paymentMethod string, plan string, price int, ets string, its string);
//                    @name('answer')
//                    SELECT
//                        a.gender AS gender1,
//                        a.price AS price1,
//                        b.price AS price2,
//                        c.price AS price3
//                    FROM pattern [
//                        every (
//                            a=SubscriptionEvent(price > 70) ->
//                            b=SubscriptionEvent(price > 70 AND gender = a.gender) ->
//                            c=SubscriptionEvent(price > 70 AND gender = a.gender)
//                            and not SubscriptionEvent(gender != a.gender)
//                        )
//                    ]
//                    """, compilerArgs);
            // Zadanie 7
            epCompiled = compiler.compile("""
                    @public @buseventtype create json schema SubscriptionEvent(gender string, age int, service string, paymentMethod string, plan string, price int, ets string, its string);
                    @name('answer')
                    SELECT
                        a.age AS first_age,
                        b.age AS second_age,
                        c.age AS third_age,
                        a.service AS service
                    FROM pattern [
                        every a=SubscriptionEvent() ->
                        b=SubscriptionEvent(service = a.service AND age > a.age) ->
                        c=SubscriptionEvent(service = a.service AND age > b.age)
                    ]
                    """, compilerArgs);
        } catch (EPCompileException ex) {
            // handle exception here
            throw new RuntimeException(ex);
        }
        return epCompiled;
    }

    static void waitToEpoch() throws InterruptedException {
        long millis = System.currentTimeMillis();
        Instant instant = Instant.ofEpochMilli(millis);
        Instant instantTrunc = instant.truncatedTo(ChronoUnit.SECONDS);
        long millis2 = instantTrunc.toEpochMilli();
        TimeUnit.MILLISECONDS.sleep(millis2 + 1000 - millis);
    }
}

