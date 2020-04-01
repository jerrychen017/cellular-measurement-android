package com.example.udp_tools;

import android.app.Activity;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.drawable.Drawable;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * TODO: document your custom view class.
 */
public class InteractiveView extends View {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private int pathIndex = 0;
    private ArrayList<Path> pathLists = new ArrayList<>();
    private ArrayList<Paint> paintLists = new ArrayList<>();
    private float startX = 0F;
    private float startY = 0F;
    private int counter = 0;
    private int last_received_sequence_num = -1;
    private int last_sent_sequence_num = -1;
    private int num_dropped_packet = 0;

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
        pathLists.add(new Path());
        paintLists.add(createPaint());
        pathIndex++;

        // initialize a socket for sending and receiving interactive packets
        initSocket("128.220.221.21", 4579);

        // setup a thread to receive packets from the socket
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    float[] coord = receiveInteractivePacket();
                    int received_seq_num = Math.round(coord[2]);
                    if (last_received_sequence_num > received_seq_num) { // received packet was delayed
                        Log.d("interactive", "Interactive packet dropped with sequence number " + received_seq_num);
                    } else {
                        Path path = pathLists.get(pathIndex - 1);
                        path.lineTo(coord[0], coord[1]);
                        counter++;
                        num_dropped_packet += (received_seq_num - last_received_sequence_num - 1);
                        last_received_sequence_num = received_seq_num;
                        Log.d("interactive", "Interactive packet received with coord x: " + coord[0] + " y: " + coord[1] + " sequence_num: " + received_seq_num);
                    }
                    InteractiveActivity.setCounter(counter, num_dropped_packet);
                }
            }
        }).start();
    }

    private Paint createPaint() {
        Paint paint = new Paint();
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(10F);
        return paint;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.drawColor(Color.CYAN);

        for (int index = 0; index < pathIndex; index++) {
            Path path = pathLists.get(index);
            Paint paint = paintLists.get(index);

            canvas.drawPath(path, paint);
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_MOVE:
                float x = event.getX();
                float y = event.getY();
//                int result = echoFromJNI("128.220.221.21", 4579, 0);
                last_sent_sequence_num++;
                int ret = sendInteractivePacket(last_sent_sequence_num, x, y);
                if (ret > 0) { // error occurred
                    Log.d("interactive", "Error occurred when sending interactive packets");
                }

//                new Thread(new Runnable() {
//                    @Override
//                    public void run() {
//                        last_sent_sequence_num++;
////                        int ret = sendInteractivePacket(last_sent_sequence_num, tempX, tempY);
////                        if (ret > 0) { // error occurred
////                            Log.d("interactive","Error occurred when sending interactive packets");
////                        }
//                        int sequence_num = last_sent_sequence_num;
//                        float[] coord = sendAndReceiveInteractivePacket(sequence_num, tempX, tempY);
//                        if (coord[0] == -1 && coord[1] == -1) { // timeout occurred!
//                            Log.e("interactive","Timeout occurred when receiving interactive packets");
//                            num_dropped_packet++;
//                            InteractiveActivity.setCounter(counter, num_dropped_packet);
//                            return;
//                        }
//
//                        if (coord[0] == -2 && coord[1] == -2) { // server name error occurred!
//                            Log.e("interactive","Server name error occurred when sending interactive packets");
////                            num_dropped_packet++;
////                            InteractiveActivity.setCounter(counter, num_dropped_packet);
//                            return;
//                        }
//                        if (coord[0] == -3 && coord[1] == -3) { // socket error occurred!
//                            Log.e("interactive","Socket error occurred when sending interactive packets");
////                            num_dropped_packet++;
////                            InteractiveActivity.setCounter(counter, num_dropped_packet);
//                            return;
//                        }
//                        if (coord[0] == -4 && coord[1] == -4) { // socket error occurred!
//                            Log.e("interactive","Not my packet error occurred when sending interactive packets");
//                            num_dropped_packet++;
//                            InteractiveActivity.setCounter(counter, num_dropped_packet);
//                            return;
//                        }
//                        last_received_sequence_num = sequence_num;
//
//                        if (sequence_num < last_received_sequence_num) { // packet delayed.
//                            // ignore this packet
//                            num_dropped_packet++;
//                            Log.d("interactive","Interactive packet dropped");
//                            InteractiveActivity.setCounter(counter, num_dropped_packet);
//                        } else {
//                            last_received_sequence_num = sequence_num;
//
//                            Path path = pathLists.get(pathIndex - 1);
//                            path.lineTo(coord[0], coord[1]);
//                            counter++;
//                            InteractiveActivity.setCounter(counter, num_dropped_packet);
//                            Log.d("interactive","Interactive packet received with coord x: " + coord[0] + " y: " + coord[1]);
//                        }
//                    }
//                }).start();
//                new Thread(new Runnable() {
//                    @Override
//                    public void run() {
//
//                            echoFromJNI("128.220.221.21", 4579, counter);
//                            Path path = pathLists.get(pathIndex - 1);
//                            path.lineTo(tempX, tempY);
//                            counter++;
//                            InteractiveActivity.setCounter(counter);
//
//                    }
//                }).start();

                break;
            default:
                break;
        }
        // Invalidate the whole view. If the view is visible.
        invalidate();
        return true;
    }

    public native int echoFromJNI(String ip, int port, int seq);

    /**
     * sends an interactive packet with coordinate x and y, and a sequence number
     *
     * @return status code, 0 if successfully sent, and 1 if failed to send
     */
    public native int sendInteractivePacket(int seq_num, float x, float y);

    /**
     * receives an interactive packet with a certain sequence number
     *
     * @param sequence_num the sequence number of the packet to be received
     * @return an array of [x_coor, y_coor]
     */
    public native float[] receiveInteractivePacket();

    public native float[] sendAndReceiveInteractivePacket(int seq_num, float x, float y);

    public native void initSocket(String address, int port);
}
