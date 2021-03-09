package com.th_nuernberg.homeekg.bluetooth_classic;

import android.app.Activity;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.Toast;
import android.widget.ToggleButton;

import androidx.annotation.WorkerThread;

import com.github.mikephil.charting.components.Legend;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.GraphView.GraphViewData;
import com.jjoe64.graphview.GraphView.LegendAlign;
import com.jjoe64.graphview.GraphViewSeries;
import com.jjoe64.graphview.GraphViewSeries.GraphViewSeriesStyle;
import com.jjoe64.graphview.LineGraphView;
import com.th_nuernberg.homeekg.Constants;
import com.th_nuernberg.homeekg.R;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.util.concurrent.atomic.AtomicBoolean;

import static com.th_nuernberg.homeekg.Constants.*;
import static com.th_nuernberg.homeekg.bluetooth_classic.BluetoothListActivity.total_bytes;
import static com.th_nuernberg.homeekg.bluetooth_classic.BluetoothListActivity.total_values;

public class SignalActivity extends Activity implements View.OnClickListener {

    //***Variables and Constants***//
    static boolean Lock = true;
    static boolean AutoScrollX = true;
    static boolean Stream = true;

    //Buttons
    Button bConnect, bDisconnect, bxMinus, bxPlus;
    ToggleButton tbLock, tbScroll, tbStream, darkMode;

    //GraphView
    static LinearLayout GraphView;
    LinearLayout background;
    static GraphView graphView;
    static GraphViewSeries Series;
    private static double graphLastXValue = 0;

    //TODO xAxis Bug Fix
    private static int xView = 30;
    private static double step = 0.05;
    BluetoothServer bluetoothServer;


    //***Methods***//
    //onCreate
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //Layout Setup
        requestWindowFeature(Window.FEATURE_NO_TITLE);


        this.getWindow().setFlags(WindowManager.LayoutParams.
                FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);
        this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        setContentView(R.layout.activity_signal);

        background = (LinearLayout) findViewById(R.id.bg);
        background.setBackgroundColor(Color.WHITE);

        //Set Handler
        BluetoothListActivity.setHandler(mHandler);

        //Initialize GraphView
        GraphView = (LinearLayout) findViewById(R.id.Graph);
        Series = new GraphViewSeries("EKG Signal",
                //Color and thickness of the line
                new GraphViewSeriesStyle(Color.RED, 4),
                new GraphViewData[] {new GraphViewData(0, 0)});

        


        graphView = new LineGraphView(this, "");
        graphView.setViewPort(0, xView);
        graphView.setScrollable(true);
        graphView.setScalable(true);
        graphView.setManualYAxis(true);
        graphView.setManualYAxisBounds(5000, 0);
        graphView.addSeries(Series);
        GraphView.addView(graphView);

        //Initialize Buttons
        bConnect = (Button)findViewById(R.id.bConnect);
        bConnect.setOnClickListener(this);

        bDisconnect = (Button)findViewById(R.id.bDisconnect);
        bDisconnect.setOnClickListener(this);

        bxMinus = (Button)findViewById(R.id.xMinus);
        bxMinus.setOnClickListener(this);

        bxPlus = (Button)findViewById(R.id.xPlus);
        bxPlus.setOnClickListener(this);

        tbLock = (ToggleButton)findViewById(R.id.tbLock);
        tbLock.setOnClickListener(this);

        tbScroll = (ToggleButton)findViewById(R.id.tbScroll);
        tbScroll.setOnClickListener(this);

        tbStream = (ToggleButton)findViewById(R.id.tbStream);
        tbStream.setOnClickListener(this);

        darkMode = (ToggleButton)findViewById(R.id.darkMode);
        darkMode.setOnClickListener(this);

        BluetoothServer bluetoothServer = new BluetoothServer();
        //bluetoothServer.start();
    }

    @Override
    protected void onDestroy() {
        if(bluetoothServer != null && bluetoothServer.isRunning()) {
            bluetoothServer.interrupt();
        }
        super.onDestroy();
    }

    //Handler
    Handler mHandler = new Handler(Looper.myLooper()){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);

            switch(msg.what){
                case SUCCESS_CONNECT:
                    BluetoothListActivity.connectedThread =
                            new BluetoothListActivity.ConnectedThread((BluetoothSocket) msg.obj);
                    Toast.makeText(getApplicationContext(),
                            "Connected!", Toast.LENGTH_SHORT).show();
                    BluetoothListActivity.connectedThread.start();
                    break;

                case MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    int numBytes = (int) msg.arg1;

                    String incomeString = new String(readBuf, 0,
                            total_bytes);
                    Log.d("Incoming String Value", incomeString);
                    Log.d("Incoming String Length", Integer.toString(numBytes));

                    incomeString = incomeString.substring(0, total_bytes-1);

                    incomeString = incomeString.replace("s", "");
                    String[] string_parts = incomeString.split("\n", total_values);
                    if(string_parts.length == total_values) {
                        for(int i = 0; i<total_values; i++) {
                            if (isIntegerNumber(string_parts[i])){
                                int yValue = Integer.parseInt(string_parts[i]);

                                if((yValue>0) && (yValue<4096) && (numBytes==total_bytes)) {
                                    Series.appendData(new GraphViewData(graphLastXValue,
                                            yValue), AutoScrollX);
                                } else {
                                    Log.d("Corrupted Bytes", Integer.toString(yValue));
                                }

                                //X-Axis Control
                                if (graphLastXValue >= xView && Lock == true){
                                    Series.resetData(new GraphViewData[] {});
                                    graphLastXValue = 0;
                                }

                                else graphLastXValue += step;

                                if(Lock == true)
                                    graphView.setViewPort(0, xView);
                                else
                                    graphView.setViewPort(graphLastXValue - xView, xView);
                            }
                            //Update
                            GraphView.removeView(graphView);
                            GraphView.addView(graphView);
                        }
                    }
                break;
            }
        }

        public boolean isIntegerNumber(String num){
            try{
                Integer.parseInt(num);
            } catch(NumberFormatException nfe) {
                return false;
            }
            return true;
        }
    };

    //onClick
    @Override
    public void onClick(View v) {
        switch(v.getId()){
            case R.id.bConnect:
                startActivity(new Intent(this, BluetoothListActivity.class));
                break;

            case R.id.bDisconnect:
                BluetoothListActivity.disconnect(this);
                break;

            case R.id.tbStream:
                if (tbStream.isChecked()){
                    if (BluetoothListActivity.connectedThread != null) {
                        byte[] message = new byte[1];
                        message[0] = 1;
                        BluetoothListActivity.connectedThread.write(message);
                    }
                } else {
                    if (BluetoothListActivity.connectedThread != null) {
                        byte[] message = new byte[1];
                        message[0] = 0;
                        BluetoothListActivity.connectedThread.write(message);
                    }
                }
                break;

            case R.id.tbScroll:
                if (tbScroll.isChecked()){
                    AutoScrollX = true;
                } else {
                    AutoScrollX = false;
                }
                break;

            case R.id.tbLock:
                if (tbLock.isChecked()){
                    Lock = true;
                } else {
                    Lock = false;
                }
                break;

            case R.id.darkMode:
                if (darkMode.isChecked()){
                    background.setBackgroundColor(Color.BLACK);
                } else {
                    background.setBackgroundColor(Color.WHITE);
                }
                break;

            case R.id.xMinus:
                if (xView > 1) xView--;
                break;

            case R.id.xPlus:
                if (xView < 30) xView++;
                break;
        }
    }

    //onBackPressed
    @Override
    public void onBackPressed() {
        if (BluetoothListActivity.connectedThread != null) {
            byte[] message = new byte[1];
            message[0] = 0;
            BluetoothListActivity.connectedThread.write(message);
        }
        super.onBackPressed();
    }

    public class BluetoothServer extends Thread {
        private Thread worker = this;
        private AtomicBoolean running = new AtomicBoolean(false);
        private AtomicBoolean stopped = new AtomicBoolean(true);
        private int interval = 4;

        public void interrupt() {
            running.set(false);
            Thread.currentThread().interrupt();
        }

        boolean isRunning() {
            return running.get();
        }

        boolean isStopped() {
            return stopped.get();
        }

        @Override
        public void run() {
            //Destroy
            running.set(true);
            stopped.set(false);

            //Buffer
            byte i[] = new byte[total_bytes];
            int pos = 0;

            //File
            String file = "RawDatenEKG.txt";
            File fileDir = new File(getFilesDir(), file);
            long fileLength = fileDir.length();

            //Loop
            while (running.get()) {
                try {
                    Thread.sleep(interval);
                } catch (InterruptedException e) {
                    interrupt();
                    e.printStackTrace();
                }
                try {
                    //READ & WRITE
                    if(fileLength > pos) {
                        FileInputStream fIn = openFileInput(file);
                        fIn.getChannel().position(pos);
                        fIn.read(i, 0, total_bytes);

                        FileOutputStream fOut = openFileOutput("EKG", MODE_APPEND);
                        fOut.write(i);
                        fOut.close();
                        pos += total_bytes;
                    } else {
                        pos = 0;
                    }

                    Message readMsg = mHandler.obtainMessage(
                            Constants.MESSAGE_READ, i.length, -1,  i);
                    readMsg.sendToTarget();
                } catch (Exception e) {
                    Log.d(TAG, "Input stream was disconnected", e);
                    break;
                }
            }
            stopped.set(true);
        }
    }
}