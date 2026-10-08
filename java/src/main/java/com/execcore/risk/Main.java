package com.execcore.risk;
public class Main { public static void main(String[] args){var r=new RiskManager(1000,16);r.applyFill("AAPL",100,180.0);System.out.println(r.position("AAPL"));} }
