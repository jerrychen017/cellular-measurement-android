package com.example.udp_tools;

public class InteractivePacket {
    public int seq;
    public float x;
    public float y;
    public int id;
    public double latency;
    public String name;


    public InteractivePacket(int seq, float x, float y, int id, double latency, String name) {
        this.seq = seq;
        this.x = x;
        this.y = y;
        this.id = id;
        this.latency = latency;
        this.name = name;
    }
}
