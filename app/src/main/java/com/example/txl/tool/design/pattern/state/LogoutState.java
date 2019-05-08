package com.example.txl.tool.design.pattern.state;

public class LogoutState implements IUserState {
    @Override
    public void comment() {
        System.out.println("进行登录，在评论");
    }
}
