package com.execcore.risk;

import java.math.BigDecimal;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.ReentrantLock;

public final class RiskManager {
    private final Map<String, Position> positions = new ConcurrentHashMap<>();
    private final ReentrantLock[] stripes;
    private final long maxAbsPosition;
    public RiskManager(long maxAbsPosition, int stripes){this.maxAbsPosition=maxAbsPosition;this.stripes=new ReentrantLock[stripes];for(int i=0;i<stripes;i++)this.stripes[i]=new ReentrantLock();}
    private ReentrantLock lock(String symbol){return stripes[(symbol.hashCode()&0x7fffffff)%stripes.length];}
    public boolean applyFill(String symbol,long qty,double price){var l=lock(symbol);l.lock();try{Position p=positions.getOrDefault(symbol,new Position(0,0,0));long next=p.quantity()+qty;if(Math.abs(next)>maxAbsPosition)return false;double avg=next==0?0:((p.averagePrice()*p.quantity())+(price*qty))/next;positions.put(symbol,new Position(next,avg,p.realizedPnl()));return true;}finally{l.unlock();}}
    public Position position(String symbol){return positions.getOrDefault(symbol,new Position(0,0,0));}
    public BigDecimal markToMarket(String symbol,double mark){Position p=position(symbol);return BigDecimal.valueOf((mark-p.averagePrice())*p.quantity());}
}
