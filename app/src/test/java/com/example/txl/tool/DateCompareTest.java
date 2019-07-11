package com.example.txl.tool;

import com.example.txl.tool.utils.DateCompare;

import junit.framework.Assert;

import org.junit.Test;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/7/11
 * description：
 */
public class DateCompareTest {
    @Test
    public void testCompare(){
        int n = DateCompare.compareDate("2019-07-10 10:50:33","2019-07-11 10:50:33","yyyy-MM-dd HH:mm:ss");
        Assert.assertEquals( n,-1 );
    }
}
