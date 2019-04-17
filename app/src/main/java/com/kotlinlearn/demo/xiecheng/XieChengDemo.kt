package com.kotlinlearn.demo.xiecheng
import kotlinx.coroutines.*
/**
 *@author TXL
 *description :协程
 */
class XieChengDemo {
    fun test1(){
        GlobalScope.launch { // 在后台启动一个新的协程并继续
            delay(1000L) // 非阻塞的等待 1 秒钟（默认时间单位是毫秒）
            println("World!") // 在延迟后打印输出
        }
        println("Hello,") // 协程已在等待时主线程还在继续
        Thread.sleep(2000L) // 阻塞主线程 2 秒钟来保证 JVM 存活
    }

    /**
     *桥接阻塞与非阻塞
     * */
    fun test2(){
        GlobalScope.launch { // 在后台启动一个新的协程并继续
            delay(1000L)
            println("World!")
        }
        println("Hello,") // 主线程中的代码会立即执行
        runBlocking {     // 但是这个表达式阻塞了主线程
            delay(2000L)  // ……我们延迟 2 秒来保证 JVM 的存活
        }
    }

    /**
     *对比test2()观察delay()与runBlocking()的区别
     * */
    fun test3(){
        GlobalScope.launch { // 在后台启动一个新的协程并继续
            delay(1000L)
            println("World!")
        }
        println("Hello,") // 主线程中的代码会立即执行
//        提示异常： Suspend function 'delay' should be called only from a coroutine or another suspend function
//        delay(2000L)
    }

    fun main() = runBlocking<Unit> { // 开始执行主协程
        GlobalScope.launch { // 在后台启动一个新的协程并继续
            delay(1000L)
            println("World!")
        }
        println("Hello,") // 主协程在这里会立即执行
        delay(2000L)      // 延迟 2 秒来保证 JVM 存活
    }

    fun testJoin(){
        val job = GlobalScope.launch { // 启动一个新协程并保持对这个作业的引用
            delay(1000L)
            println("World!")
        }
        println("Hello,")
        runBlocking {
            job.join() // 等待直到子协程执行结束
        }
//        不能再这个位置直接调用
//        job.join() // 等待直到子协程执行结束
    }

    fun testJoinRunBlocking(){
        runBlocking<Unit>{
            val job = GlobalScope.launch { // 启动一个新协程并保持对这个作业的引用
                delay(1000L)
                println("World!")
            }
            println("Hello,")
            job.join() // 等待直到子协程执行结束
        }
    }

    /**
     *结构化的并发
     * 这样来理解，在协程中创建协程，当前协程会等到当前协程中的所有其它协程结束后才会结束当前协程
     * */
    fun runBlocking() = runBlocking { // this: CoroutineScope
        launch { // 在 runBlocking 作用域中启动新协程
            delay(1000L)
            println("World!")
        }
        println("Hello,")
    }

    fun runBlocking1() = runBlocking {
        //这样创建的是全局协程，即使全局协程未结束，如果当前协程执行完也会结束
        GlobalScope.launch { // 在 runBlocking 作用域中启动新协程
            delay(1000L)
            println("World!")
        }
        println("Hello,")
    }

    /**
     * 作用域构建器
     * coroutineScope后面的代码会在coroutineScope中的协程执行完成之后在执行，换句话说他会阻塞当前协程？
     * 而runBlocking会把当前协程中的内容执行完毕进行等待
     * */
    fun mainArea() = runBlocking { // this: CoroutineScope
        launch {
            delay(200L)
            println("Task from runBlocking")
        }
        println("Coroutine scope will start ")
        coroutineScope { // 创建一个新的协程作用域
            launch {
                delay(500L)
                println("Task from nested launch")
            }

            delay(100L)
            println("Task from coroutine scope") // 这一行会在内嵌 launch 之前输出
        }

        println("Coroutine scope is over") // 这一行在内嵌 launch 执行完毕后才输出,指的是：coroutineScope中的 launch
    }
}
fun main(args: Array<String>){
    val xieChengDemo = XieChengDemo()
    xieChengDemo.mainArea()
}

