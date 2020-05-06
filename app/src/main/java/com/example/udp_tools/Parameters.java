package com.example.udp_tools;

public class Parameters {
    int burstSize;    // number of packets in increased speed burst
    int intervalSize; // one burst per INTERVAL_SIZE packets, should make this a multiple of BURST_SIZE
    double intervalTime;
    int instantBurst;
    double minSpeed;
    double maxSpeed;
    double startSpeed;
    int gracePeriod;
    int predMode;
    int useTCP;
    double alpha;
    double threshold;

    public Parameters(int burstSize, int intervalSize, double intervalTime, int instantBurst, double minSpeed, double maxSpeed, double startSpeed, int gracePeriod, int predMode, int useTCP, double alpha, double threshold) {
        this.burstSize = burstSize;
        this.intervalSize = intervalSize;
        this.intervalTime = intervalTime;
        this.instantBurst = instantBurst;
        this.minSpeed = minSpeed;
        this.maxSpeed = maxSpeed;
        this.startSpeed = startSpeed;
        this.gracePeriod = gracePeriod;
        this.predMode = predMode;
        this.useTCP = useTCP;
        this.alpha = alpha;
        this.threshold = threshold;
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

    public int getPredMode() {
        return predMode;
    }

    public int getUseTCP() {return useTCP;}

    public double getAlpha() {
        return alpha;
    }

    public double getThreshold() {
        return threshold;
    }
}
