package com.example.txl.tool.design.pattern.state;

public class UserStateContext {
    private static UserStateContext userStateContext;
    private IUserState mState;

    private UserStateContext(){

    }

    public static UserStateContext getUserStateContext(){
        if(userStateContext == null){
            userStateContext = new UserStateContext();
            userStateContext.mState = new LogoutState();
        }
        return userStateContext;
    }

    public static UserStateContext getUserStateContext(IUserState userState){
        userStateContext = getUserStateContext();
        userStateContext.mState = userState;
        return userStateContext;
    }

    public void setmState(IUserState mState) {
        this.mState = mState;
    }

    public IUserState getmState() {
        return mState;
    }
}
