const {Interceptor}=require("interceptor-gamepad-controller");const io=new Interceptor();try{console.log(io.info())}finally{io.close()}
