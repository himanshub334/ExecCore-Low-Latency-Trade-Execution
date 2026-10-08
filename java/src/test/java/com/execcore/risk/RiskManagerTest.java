package com.execcore.risk;
import org.junit.jupiter.api.Test;import java.util.concurrent.*;import static org.junit.jupiter.api.Assertions.*;
class RiskManagerTest{
 @Test void rejectsLimit(){var r=new RiskManager(100,8);assertTrue(r.applyFill("X",100,10));assertFalse(r.applyFill("X",1,10));}
 @Test void stripedUpdatesStayAtomic() throws Exception{var r=new RiskManager(1000,16);var ex=Executors.newFixedThreadPool(8);var fs=new java.util.ArrayList<Future<?>>();for(int i=0;i<8;i++)fs.add(ex.submit(()->{for(int j=0;j<100;j++)assertTrue(r.applyFill("X",1,10));}));for(var f:fs)f.get();ex.shutdown();assertEquals(800,r.position("X").quantity());}
}
