package com.example.udp_tools;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

/**
 * A view class that's responsible for drawing interactions
 */
public class InteractiveView extends View {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private int counter = 0;
    private int last_received_sequence_num = -1;
    private int last_sent_sequence_num = -1;
    private double latency = 0;
    private int num_dropped_packet = 0;

    private int myID;
    private int maxUsers = 10;
    private InteractiveUser[] users;
    private InteractiveUser realMyUser;
    private boolean isConnected = false;

    public InteractiveView(Context context) {
        super(context);
        init(null, 0);
    }

    public InteractiveView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(attrs, 0);
    }

    public InteractiveView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(attrs, defStyle);
    }


    private void init(AttributeSet attrs, int defStyle) {
        users = new InteractiveUser[maxUsers];
        // initialize users to null
        for (int i = 0; i < maxUsers; i++) {
            users[i] = null;
        }
    }

    public void connect(String address, int port, String name) {
        // initialize a socket for sending and receiving interactive packets
        int id = initInteractive(address, port, name);

        if (id < 0) {
            System.err.println("Error occurred when connecting to user");
        } else {
            myID = id;
            // new interactive user for myself
            users[myID] = new InteractiveUser(myID ,name, 0, 0, getWidth());
            realMyUser = new InteractiveUser(myID, "", 0, 0, getWidth());
            realMyUser.circlePaint.setAlpha(100);
        }
        // setup a thread to receive packets from the socket
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    InteractivePacket pkt = receiveInteractivePacket();
                    System.out.println("received an interactive packet");
                    int received_seq_num = pkt.seq;
                    int received_id = pkt.id;
                    if (received_id == myID) {
                        if (last_received_sequence_num > received_seq_num) { // received packet was delayed
//                            Log.d("interactive", "Interactive packet dropped with sequence number " + received_seq_num);
                        } else {
                            users[myID].setX(pkt.x);
                            users[myID].setY(pkt.y);
                            counter++;
                            num_dropped_packet += (received_seq_num - last_received_sequence_num - 1);
                            last_received_sequence_num = received_seq_num;
                            latency = pkt.latency;
//                            Log.d("interactive", "Interactive packet received with coord x: " + pkt.x + " y: " + pkt.y + " sequence_num: " + received_seq_num);
                        }
                        MainActivity.updateStat(counter, num_dropped_packet, latency);
                    } else { // other user
                        boolean userFound = false;
                        for (InteractiveUser usr : users) {
                            if (usr != null && usr.id == received_id) {
                                userFound = true;
                                usr.setX(pkt.x);
                                usr.setY(pkt.y);
                                usr.setName(pkt.name);
                            }
                        }
                        if (!userFound) {
                            users[received_id] = new InteractiveUser(received_id, pkt.name, pkt.x, pkt.y, getWidth());
                        }
                    }
                    invalidate();

                }
            }
        }).start();
        isConnected = true;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.drawColor(Color.CYAN);

        if (realMyUser != null) {
            float x = realMyUser.x * getWidth();
            float y = realMyUser.y * getHeight();
            canvas.drawCircle(x, y, getWidth()/20, realMyUser.circlePaint);
        }

        for (int i = 0; i < maxUsers; i++) {
            if (users[i] != null) {
                InteractiveUser usr = users[i];
                float x = usr.x * getWidth();
                float y = usr.y * getHeight();
                canvas.drawCircle(x, y, getWidth()/20, usr.circlePaint);
                canvas.drawText(usr.name, x, y, usr.textPaint);
            }
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_MOVE:
            case MotionEvent.ACTION_DOWN:
                if (isConnected) {
                    last_sent_sequence_num++;
                    realMyUser.x = event.getX()/getWidth();
                    realMyUser.y = event.getY()/getHeight();

                    int ret = sendInteractivePacket(last_sent_sequence_num, event.getX()/getWidth(), event.getY()/getHeight());
                    if (ret > 0) { // error occurred
//                        Log.d("interactive", "Error occurred when sending interactive packets");
                    }
                }
                break;
            default:
                break;
        }
        // Invalidate the whole view. If the view is visible.
        invalidate();
        return true;
    }

    /**
     * sends an interactive packet with coordinate x and y, and a sequence number
     *
     * @return status code, 0 if successfully sent, and 1 if failed to send
     */
    public native int sendInteractivePacket(int seq_num, float x, float y);

    /**
     * receives an interactive packet with a certain sequence number
     *
     * @return Interactive Packet
     */
    public native InteractivePacket receiveInteractivePacket();


    /**
     * Set up socket to the server and send a CONNECT packet to the server with a name.
     * Wait for the server to respond with a CONNECT packet containing an id. If any error occurred,
     * returned id would be negative indicating an error
     * @param address server address
     * @param port server port
     * @param name my user name
     * @return id
     */
    public native int initInteractive(String address, int port, String name);
}
