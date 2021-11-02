package com.autoipod.jni;

public interface IPcmDataReceiveCallBack {
    public void OnPcmDataReceive(int datalen,byte data[]);
}
