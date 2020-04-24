package com.example.udp_tools;

public class Parameters {
    int burstSize;    // number of packets in increased speed burst
    int intervalSize; // one burst per INTERVAL_SIZE packets, should make this a multiple of BURST_SIZE
    double intervalTime;
    int instantBurst;
    int burstFactor;
    double minSpeed;
    double maxSpeed;
    double startSpeed;
    int gracePeriod;

    public Parameters(int burstSize, int intervalSize, double intervalTime, int instantBurst, int burstFactor, double minSpeed, double maxSpeed, double startSpeed, int gracePeriod) {
        this.burstSize = burstSize;
        this.intervalSize = intervalSize;
        this.intervalTime = intervalTime;
        this.instantBurst = instantBurst;
        this.burstFactor = burstFactor;
        this.minSpeed = minSpeed;
        this.maxSpeed = maxSpeed;
        this.startSpeed = startSpeed;
        this.gracePeriod = gracePeriod;
    }

    public int getBurstSize() {
        return burstSize;
    }

    public int getIntervalSize() {
        return intervalSize;
    }

    public double getIntervalTime() {
        return intervalTime;
    }

    public int getInstantBurst() {
        return instantBurst;
    }

    public int getBurstFactor() {
        return burstFactor;
    }

    public double getMinSpeed() {
        return minSpeed;
    }

    public double getMaxSpeed() {
        return maxSpeed;
    }

    public double getStartSpeed() {
        return startSpeed;
    }

    public int getGracePeriod() {
        return gracePeriod;
    }
}
