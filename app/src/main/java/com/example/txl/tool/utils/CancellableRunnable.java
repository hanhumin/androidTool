package com.example.txl.tool.utils;

public abstract class CancellableRunnable implements Runnable {
    private volatile boolean _isCancelled = false;

    public void cancel() {
        _isCancelled = true;
    }

    @Override
    public void run() {
        if (_isCancelled) {
            return;
        }
        doRun();
    }

    protected abstract void doRun();
}