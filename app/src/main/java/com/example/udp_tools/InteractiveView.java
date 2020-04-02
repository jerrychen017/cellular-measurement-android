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
    private int counter = 0;
    private int last_received_sequence_num = -1;
    private int last_sent_sequence_num = -1;
    private int num_dropped_packet = 0;

    private float xcoord;
    private float ycoord;

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
        initInteractive("128.220.221.21", 4579, "test-name", 7);

        // setup a thread to receive packets from the socket
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    InteractivePacket pkt = receiveInteractivePacket();
                    System.out.println("received an interactive packet");
                    int received_seq_num = pkt.seq;
                    if (last_received_sequence_num > received_seq_num) { // received packet was delayed
                        Log.d("interactive", "Interactive packet dropped with sequence number " + received_seq_num);
                    } else {
                        Path path = pathLists.get(pathIndex - 1);
                        path.lineTo(pkt.x, pkt.y);
                        xcoord = pkt.x;
                        ycoord = pkt.y;
                        counter++;
                        num_dropped_packet += (received_seq_num - last_received_sequence_num - 1);
                        last_received_sequence_num = received_seq_num;
                        Log.d("interactive", "Interactive packet received with coord x: " + pkt.x + " y: " + pkt.y + " sequence_num: " + received_seq_num);
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
        Paint paint = new Paint();
        paint.setColor(Color.BLUE);
        canvas.drawCircle(xcoord, ycoord, 100,paint);

//        for (int index = 0; index < pathIndex; index++) {
//            Path path = pathLists.get(index);
//            Paint paint = paintLists.get(index);
//
//            canvas.drawPath(path, paint);
//            canvas.drawPath(path, paint);
//        }
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
     * @return Interactive Packet
     */
    public native InteractivePacket receiveInteractivePacket();


    public native void initInteractive(String address, int port, String name, int id);
}
