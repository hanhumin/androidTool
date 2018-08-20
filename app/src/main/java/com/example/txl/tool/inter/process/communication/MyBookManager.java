package com.example.txl.tool.inter.process.communication;

import android.os.IBinder;
import android.os.RemoteException;

import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/14
 * description：手写Binder
 */
public interface MyBookManager extends android.os.IInterface {


    public List<Book> getBookList() throws RemoteException;
    public void addBook(Book book) throws RemoteException;
}
