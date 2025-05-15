package com.sample;

import org.kie.api.KieServices;
import org.kie.api.runtime.KieContainer;
import org.kie.api.runtime.KieSession;
import org.kie.api.logger.*;

public class DroolsTest {

	public static final void main(String[] args) {
        try {
            // load up the knowledge base
	        KieServices ks = KieServices.Factory.get();
    	    KieContainer kContainer = ks.getKieClasspathContainer();
        	KieSession kSession = kContainer.newKieSession("ksession-rules");
        	KieRuntimeLogger kLogger = ks.getLoggers().newFileLogger(kSession,  "test");
            // go !
            Straz s = new Straz();
            kSession.insert(s);
            kSession.fireAllRules();
            kLogger.close();
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }
    
    public static class Pozar {
    	public Pozar() {
    		
    	}
    }
    
    public static class Telefon {
    	public long numer;
    	public Telefon(long n) {
    		this.numer = n;
    	}
    }

    public static class Pomiar {

        public boolean dym;
        public int temperatura;
        
        public Pomiar(boolean dym, int t) {
        	this.dym = dym;
        	this.temperatura = t;
        }

    }
    
    public static class Straz {
    	public Straz() {
    		
    	}
    }

}
