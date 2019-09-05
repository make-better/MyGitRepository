window.onload = init;
window.onmousemove = mouseMoveHandler;

var context;
//各种图形变量
var pe_l,pe_y,sun,cbg,wbg,zc,sta,bg1,bg2,ball,board;
//随机道具表
var gift = [], dgift = [];
//游戏总开关
var gameon = false;
//生命状态
var life = 3;//0:正在玩；1:赢了;2:死了;3:在跳转界面
//是否显示"开始游戏"字体背景
var diswgd = false;
//主循环暂存
var time = 0;
//转场定时器启动标志位
var changeflag = true;

//帧数
var fps = 60;
//重新游戏时间
var restart = 6000;
//体力条和血量
var energy = 100, blood = 120;
//体力恢复控制位
var energyup = true;
//体力恢复单位
var eunit = 0.15;
//特效参数
var PE_L = 0,PE_Y = 0,alp = 0,alp_flag = 0;
//总分
var total = 0;
//canvas的宽和高
var cW=1024,cH=768;
//挡板坐标
var boardX=512-100,boardY=720;
//小球坐标
var ballX=400, ballY=450;
//小球大小
var ballW = 30, ballH = 30;
//小球速度
var vx = 4, vy = 4;
//小球速度极限
var VMAX = 10, VMIN = 4;
//砖块的大小
var brickW = 79, brickH = 63;
//砖块的数量
var nomofline = 4, nomofcol = 10;
//道具的大小
var giftW = 50, giftH = 50;
//声明并初始化一个新的数组
var bricks=[];
//这是最重要的canvas
var canvas=document.getElementById("game");
//鼠标位置修正值
var size;
var xm=(window.innerWidth-cW)/2,ym=0;
//判断鼠标点击位置
function getEventPosition(ev){ 　
	var x, y; 
	if (ev.layerX || ev.layerX == 0) 
	{ 　　
		x = ev.layerX; 　　
		y = ev.layerY; 　　
	} else if (ev.offsetX || ev.offsetX == 0)
 	{
		x = ev.offsetX; 　
	 	y = ev.offsetY; 
	} 　
	if(btnlogin()=="Firefox")//为了解决兼容性问题
 		return {x: (x-xm), y: ((y-ym)*size)};
 	else if(btnlogin()=="Safari") 
 		return {x: (x), y: ((y-ym))};
}
//鼠标点击事件
function mouseclick(e){
	//log(getEventPosition(e)); 
	//log("width"+window.screen.width*detectZoom());
	log("浏览器:"+btnlogin());
	log("width"+window.screen.width);
	log("canvas width"+canvas.width);
	//log("height"+window.screen.availHeight );
	log("xm"+xm );
	var p=getEventPosition(e);
	//判断是否点击了“开始游戏” 
	if(p.x>=409&&p.x<=409+206&&
	   p.y>=407&&p.y<=(407+44)){
	   	if(life == 3)
	   	{
	   		//关闭背景音乐1
			closePlay();
			gameon = true;
	   	}
	}
}
//查看浏览器内核类型
function btnlogin()
{
     if(navigator.userAgent.indexOf("MSIE")>0) {                                 // MSIE内核        
            return "MSIE"; 
        }
    if(navigator.userAgent.indexOf("Firefox")>0){                                 // Firefox内核       
            return "Firefox"; 
        }
    if(navigator.userAgent.indexOf("Opera")>0){                                  // Opera内核       
            return "Opera"; 
        }
    if(navigator.userAgent.indexOf("Safari")>0) {                                  // Safari内核       
            return "Safari";  
        } 
    if(navigator.userAgent.indexOf("Camino")>0){                                  // Camino内核       
            return "Camino"; 
        } 
    if(navigator.userAgent.indexOf("Gecko")>0){                                    // Gecko内核       
            return "Gecko"; 
        } 
}

//获取当前页面的缩放值1.x
function detectZoom(){
	var ratio = 0;
		screen = window.screen;
		ua = navigator.userAgent.toLowerCase();
		if(window.devicePixelRatio != undefined){
			ratio = window.devicePixelRatio;
		}else if(~ua.indexOf('msie')){
			if(screen.deviceXDPI && screen.logicalXDPI){
				ratio = screen.deviceXDPI/screen.logicalXDPI;
			}
		}
		else if(window.outerWidth != undefined && window.innerWidth != undefined){
			ratio = window.outerWidth/window.innerWidth;
		}
		if(ratio){
			ratio = Math.round(ratio*100);
		}
		return ratio/100;
}
//鼠标移动事件
function mousemoved(e){ 
	console.log(getEventPosition(e)); 
	var p=getEventPosition(e);
	//鼠标移动到一个范围
	if(p.x>=409-106&&p.x<=409+206+106&&
	   p.y>=407&&p.y<=407+88){
	   	if(diswgd == true)
	   		return;
	   	diswgd = true;
	    //音效
	   	auto_Qierou_Play();
	}
	else{
		if(diswgd == false)
			return;
		auto_Qierou_Play();
		diswgd = false;
	}
}

//初始化
function init() {
	
	context=canvas.getContext("2d");
	//添加鼠标点击事件
	canvas.addEventListener('click',mouseclick,false);
	//添加鼠标移动事件，移动到“开始游戏”上就显示背景
	canvas.addEventListener('mousemove',mousemoved,false);

	bg2 = addImage("assets/bg2.png");
	board=addImage("assets/board.png");
	ball = addImage("assets/ball2.png");
	cbg = addImage("assets/cbg.png");
	sun = addImage("assets/sun.png");
	//生成砖块
	creatBricks();
	//播放背景音乐1
	autoPlay();
	//添加循环机制
	time = setInterval(gameTick,1000/fps);//clearInterval(time);
}
//游戏循环逻辑
function gameTick(){
	//清屏
	clearScreen();
	size = 860/window.screen.availHeight/detectZoom();
	xm=(window.screen.width-cW)/2;//*(window.screen.width*detectZoom()/1600)
	ym=0;
	if(gameon == false)
	{
		//进入开始界面
		starts();
	}//正式开始游戏
	else
	if(gameon == true){
		diswgd = false;
		//删除鼠标监听事件
		canvas.removeEventListener('mousemove',mousemoved,false);
		//绘制背景
		context.drawImage(bg2,0,0);
		//绘制分数背景
		context.drawImage(cbg,cW-150,cH-100);
		//显示总分
		drawpoint();
		//显示玩家状态
		displayer();
		if(life == 3){//进入跳转界面
			//绘制转场背景
			context.drawImage(zc,0,0);
			if(changeflag == true){//只运行一次
				//5s后开始游戏
				setTimeout(function(){
									life = 0;
									//播放背景音乐2
									autoPlay2();
									},5000);
				changeflag = false;
			}			
		}else
		if(life == 0)//游戏进行中
		{
			//绘制挡板
			context.drawImage(board,boardX,boardY);
			//产生粒子效果
			Particle_Effects();
			//绘制砖块
			updateBricks();
			//绘制随机道具
			drawgifts();
			//绘制小球
			updateBall();
			//小球和挡板的碰撞检测
			testBallAndBoard();
			//小球和砖块的碰撞检测
			testBallAndBricks();
			//随机道具与挡板的碰撞检测
			testGiftAndBoard();
			//体力恢复
			if(energy<100 && energyup == true)
				energy += eunit;
		}
		//游戏结束有两种状况，一种是赢了，另一种是死了
		else if(life == 1){
			The_Last_Effect();
		}else if(life == 2){
			The_Last_Effect();
		}
	}
}
//开始界面
function starts(){
	//背景
	bg1 = addImage("assets/bg1.png");
	context.drawImage(bg1,0,0);
	//转场
	zc = addImage("assets/zhuanchang.png");
	//开始按钮
	sta = addImage("assets/sta.png");
	sta.width = 206;
	sta.height = 44;
	context.drawImage(sta,409,407);
	//绘制字体背景
	if(diswgd == true)
	{
		wbg = addImage("assets/wbg.png");
		context.drawImage(wbg,409-106,407-22);
	}
}
//小球和砖块的碰撞检测
function testBallAndBricks(){
	var D_value_X = 0;
	var D_value_Y = 0;
	for(var i=bricks.length-1;i>=0;i--){
		var item = bricks[i];
		//判断撞击是否发生
		if(Math.abs((ballX + ballW/2) - (item.x + brickW/2)) < (brickW + ballW)/2 &&
	        Math.abs((ballY + ballH/2) - (item.y + brickH/2)) < (ballH + brickH)/2){

			D_value_X = ((ballX + ballW/2) - (item.x + brickW/2))/brickW;
			D_value_Y = ((ballY + ballH/2) - (item.y + brickH/2))/brickH;
			if(Math.abs(D_value_X)<=Math.abs(D_value_Y)){//碰撞发生在上方或下方
				if(D_value_Y>0){//碰撞发生在下方
					vy = (VMAX+VMIN)/2;
				}else{
					vy = -1*(VMAX+VMIN)/2;
				}
			}else{
				if(D_value_X<0){//碰撞发生在左侧
					vx = -1*(VMAX+VMIN)/2;
				}else{
					vx = (VMAX+VMIN)/2;
				}
			}
			//删除此砖块
			bricks.splice(i,1);
			total++;
			auto_Tiaozhan_Play();
			//产生道具
			if(item.num != 0){//图片				X 					Y 						种类	
				dgift.push({g:gift[item.num-1].gifp,x:item.x+brickW/2-25,y:item.y+brickH/2-25,kind:gift[item.num-1].kind});
			}
		}
	}
	//砖块被敲碎完，游戏胜利
	if(bricks.length == 0){
		closePlay2();
		auto_Shengli_Play();
		life = 1;
		//5s后重新开始游戏
		setTimeout(function(){window.history.go(0);},restart);
	}
}
//画出随机道具
function drawgifts(){
	if(dgift.length > 0){
		for(var pp=0;pp<dgift.length;pp++){
			context.drawImage(dgift[pp].g,dgift[pp].x,dgift[pp].y);
			//更新道具的位置
			dgift[pp].y+=2;
			//删除掉落出屏幕的道具
			if(dgift[pp].y > cH){
				dgift.splice(pp,1);
			}
		}
	}
}
//随机道具与挡板的碰撞检测
function testGiftAndBoard(){
	for(var tg = 0;tg < dgift.length;tg++){
		var hit = hitTestPoint(boardX-giftW,boardY-giftH,board.width+giftW,board.height+giftH,dgift[tg].x,dgift[tg].y);
		if(hit == true){
			switch(dgift[tg].kind){
				//随机道具是绿花草
				case 1:;
				case 2:eunit = 0.3;
						//10s后绿条回复量恢复常态
						window.setTimeout(function(){eunit = 0.15;board=addImage("assets/board.png");}, 10000);
						//更改挡板贴图
						board=addImage("assets/board2.png");
						auto_Lvhuacao_Play();
						dgift.splice(tg,1);
						break;
				//随机道具是杰克的酒
				case 3:;
				case 4:;
				case 5:blood += 40;
						if(blood>120){
							blood = 120;
						}
						dgift.splice(tg,1);
						auto_Yuansuping_Play();
						//启动粒子效果
						PE_Y = 1;
						break;
				default:break;
			}
			hit = 0;
		}
	}
}
//小球和挡板的碰撞检测
function testBallAndBoard(){
	var hit = hitTestPoint(boardX-ballW,boardY-ballH,board.width+ballW,board.height+ballH,ballX,ballY);
	if(hit){
		ballY = boardY- ballH;
		//对速度在进行更改
		playtheball();
	}
}
//简单碰撞检测
function hitTestPoint(x1,y1,w1,h1,x2,y2){
	if(x2>=x1 && x2<=x1+w1&&
		y2>=y1 && y2<=y1+h1
		)
		return true;
	else 
		return false;
}

//显示砖块
function updateBricks(){
	for(var i=0;i<bricks.length;i++){
		var item = bricks[i];
		context.drawImage(item.item,item.x,item.y);
	}
}

//创建砖块
function creatBricks(){
	var type = 1;
	//随机物品的基础位置
	var base = random(0,nomofline*nomofcol);
	var list = new Array(7,11,24,27,42)
	//生成随机物品
	for(var nn = 1;nn<=5;nn++){
		gift.push({n:(base+list[nn-1])%(nomofcol*nomofline),//所处位置
					kind:nn,							//种类
					gifp:addImage("assets/gift"+nn+".png")//图片
				});
	}
	//计算合适的初始位置
	var xx =  (cW-((nomofcol)*brickW))/2-(nomofcol-1)*4;
	for(var i=0;i<nomofcol;i++){
		for(j=0;j<nomofline;j++){
			var item = addImage("assets/b"+random(1,18)+".png");
			//向砖块中塞入物品,并生成砖块的信息
			for(var o=1;o<=gift.length;o++){
				if(((i*nomofline)+j+1)==gift[o-1].n){//可以把道具塞入砖块
					type = 1;
					break;
				}
				else{//不把道具塞入砖块
					type = 0;
				}
			}
			if(type == 1)
				bricks.push({item:item,x:xx+(brickW+5)*i,y:100+(brickH+4)*j,num:o});
			else
				bricks.push({item:item,x:xx+(brickW+5)*i,y:100+(brickH+4)*j,num:0});
		}
	}
}

//更新小球坐标
function updateBall(){
	ballX+=vx;
	ballY+=vy;
	//log("小球运动");
	if(ballX<0){
		ballX = 0;
		vx*=-1;
	}else if(ballX>cW - ball.width){
		ballX = cW-ball.width;
		vx*=-1;
	}

	if(ballY<0){
		ballY = 0;
		vy*=-1;
	}else if(ballY>760){//小球掉出了屏幕
		window.setTimeout(closePlay2(), 500);
		auto_Youdied_Play();
		log("YOU DIED");
		life = 2;//你死啦
		//5s后重新开始游戏
		setTimeout(function(){window.history.go(0);},restart);
		return 1;
	}
	context.drawImage(ball,ballX,ballY);
}
//显示总分
function drawpoint(){
	context.fillStyle='#aaa';
	context.font="18px 仿宋_GB2312";
	context.fillText(" "+total+" ",cW-40,cH-87+20);
}
//鼠标移动时更新挡板坐标
function mouseMoveHandler( e ){
	if(gameon == false)
		return 0;
	boardX = e.x- board.width/2-xm;
	if(boardX <= 1)
		boardX = 0;
	if(boardX>=cW-board.width)
		boardX = cW-board.width;
	//调试部分
	//log("小球坐标更新");
	//ballX = e.x - 25-xm;
	//ballY = e.y - 25;
	//
}
//显示玩家状态
function displayer(){
	//画出太阳战士信仰
	context.drawImage(sun,30,10);
	//画底板
	context.beginPath();
	context.lineWidth = 10;
	context.strokeStyle='#767682';
	context.moveTo(71+30,33);
	context.lineTo(71+30+120*3,33);
	context.globalAlpha = 0.4;
	context.stroke();
	context.globalAlpha = 1;
	context.closePath();
	//画血条
	context.beginPath();
	context.lineWidth = 10;
	context.strokeStyle='#753a40';
	context.moveTo(71+30,33);
	context.lineTo(71+30+blood*3,33);
	context.stroke();
	context.closePath();
	//描边
	context.beginPath();
	context.lineWidth = 1;
	context.strokeStyle='#656571';
	context.moveTo(71+30-2,28);
	context.lineTo(71+30+120*3+2,28);
	context.stroke();
	context.moveTo(71+30-2,28+10);
	context.lineTo(71+30+120*3+5,28+10);
	context.stroke();
	context.rect(70+30,28, 120*3+1, 11);
	context.stroke();
	context.closePath();
	//画底板
	context.beginPath();
	context.lineWidth = 10;
	context.strokeStyle='#767682';
	context.moveTo(71+30,53);
	context.lineTo(71+30+100*3,53);
	context.globalAlpha = 0.4;
	context.stroke();
	context.globalAlpha = 1;
	context.closePath();
	//画绿条
	context.beginPath();
	context.lineWidth = 10;
	context.strokeStyle='#3b6245';
	context.moveTo(71+31,53);
	context.lineTo(71+31+energy*3,53);
	context.stroke();
	context.closePath();
	//描边
	context.beginPath();
	context.lineWidth = 1;
	context.strokeStyle='#656571';
	context.moveTo(71+31-2,48);
	context.lineTo(71+31+100*3+2,48);
	context.stroke();
	context.moveTo(71+31-2,48+10);
	context.lineTo(71+31+100*3+5,48+10);
	context.stroke();
	context.rect(70+31,48,100*3+1, 11);
	context.stroke();
	context.closePath();
}
//玩家状态控制
function playerlife(h1,h2){	
	//掉体力部分
	if(h2>0)
		energy -= 10+h2;
	else
		energy += h2-10;

	if(energy<=0){
		
		energy = 0;
	}

	//掉血部分
	if(energy<=0){
		auto_Jikui_Play();
		blood-=h1*2;
	}
	else
	{
		auto_Tanfan_Play();
		blood-=h1;
	}
	if(blood<=0){
		//停止播放bgm
		window.setTimeout(closePlay2(), 500);
		auto_Youdied_Play();
		//5s后重新开始游戏
		setTimeout(function(){window.history.go(0);},5000);
		blood = 0;
		life = 2;
	}
}
//小球与挡板的接触控制
function playtheball(){
	var temp = 0;
	temp = (ballX+ballW)-(boardX+100);
	vx+=(temp/10);
	vy*=(-1)*(1+(random(1,3)/10));
	//碰撞导致体力暂停恢复一段时间
	energyup = false;
	setTimeout(function(){energyup = true;},400);
	//更改玩家状态
	playerlife(-vy,vx*2);
	if(vx>0){
		if(vx>VMAX)
		vx = VMAX;
		if(vx<VMIN)
		vx = VMIN;
	}
	if(vx<0){
		if(vx<-VMAX)
		vx = -VMAX;
		if(vx>-VMIN)
		vx = -VMIN;
	}
	
	if(vy>0){
		if(vy>VMAX)
		vy = VMAX;
		if(vy<VMIN)
		vy = VMIN;
	}
	if(vy<0){
		if(vy<-VMAX)
		vy = -VMAX;
		if(vy>-VMIN)
		vy = -VMIN;
	}
}
//清屏
function clearScreen(){
	context.clearRect(0,0,cW,cH);
}

//添加图片
function addImage(url){
	var img = new Image();
	img.src=url;
	return img;
}
//输出log到控制台
function log(msg) {
	console.log(msg);
}

function random(min,max){
	var result = min+(max-min+1)*Math.random();
	return parseInt(result);
}
//声音控制部分
function autoPlay() {
            var myAuto = document.getElementById('myaudio');
            myAuto.volume = 0.2;
            myAuto.play();
        }
function closePlay() {
            var myAuto = document.getElementById('myaudio');
            myAuto.pause();
            myAuto.load();
        }
function autoPlay2() {
            var myAuto = document.getElementById('myaudio2');
            myAuto.volume = 0.2;
            myAuto.play();
        }
function closePlay2() {
            var myAuto = document.getElementById('myaudio2');
            myAuto.pause();
           // myAuto.load();
        }
function auto_Lvhuacao_Play() {
            var myAuto = document.getElementById('lvhuacao');
            myAuto.pause();
            myAuto.load();
            myAuto.volume = 1;
            myAuto.play();
        }
function auto_Yuansuping_Play() {
            var myAuto = document.getElementById('yuansuping');
            myAuto.pause();
            myAuto.load();
            myAuto.volume = 1;
            myAuto.play();
        }
function auto_Tanfan_Play() {
            var myAuto = document.getElementById('tanfan');
            myAuto.pause();
            myAuto.load();
            myAuto.volume = 0.7;
            myAuto.play();
        }
function auto_Qierou_Play() {
            var myAuto = document.getElementById('qierou');
            myAuto.volume = 0.7;
            myAuto.play();
        }
function auto_Tiaozhan_Play() {
            var myAuto = document.getElementById('tiaozhan');
            // myAuto.pause();
            // myAuto.load();
            myAuto.volume = 1;
            myAuto.play();
        }
function auto_Jikui_Play() {
            var myAuto = document.getElementById('jikui');
            myAuto.pause();
            myAuto.load();
            myAuto.volume = 1;
            myAuto.play();
        }
function auto_Shengli_Play() {
            var myAuto = document.getElementById('shengli');
            myAuto.volume = 1;
            myAuto.play();
        }
function auto_Youdied_Play() {
            var myAuto = document.getElementById('youdied');
            myAuto.volume = 0.8;
            myAuto.play();
        }
//粒子效果
function Particle_Effects(){
	if(eunit != 0.15){
			if(PE_L<fps/6)
				{pe_l = addImage("assets/l1.png");}
			else if(PE_L<fps/6*2)
				{pe_l = addImage("assets/l2.png");}
			else if(PE_L<fps/6*3)
				{pe_l = addImage("assets/l3.png");}
			else if(PE_L<fps/6*4)
				{pe_l = addImage("assets/l4.png");}
			else if(PE_L<fps/6*5)
				{pe_l = addImage("assets/l5.png");}
			else
				PE_L = 0;
			PE_L++;
			context.drawImage(pe_l,boardX,boardY-6);
	}
	if((PE_Y != 0)&&(PE_Y < 100)){
		//log("PE_Y:"+PE_Y);
		//log(parseInt(PE_Y/(fps/10)));
		switch(parseInt(PE_Y/(fps/10))){
			case 0:pe_y = addImage("assets/g1.png");break;
			case 1:pe_y = addImage("assets/g2.png");break;
			case 2:pe_y = addImage("assets/g3.png");break;
			case 3:pe_y = addImage("assets/g4.png");break;
			case 4:pe_y = addImage("assets/g5.png");break;
			case 5:pe_y = addImage("assets/g6.png");break;
			case 6:pe_y = addImage("assets/g7.png");break;
			case 7:pe_y = addImage("assets/g8.png");break;
			case 8:pe_y = addImage("assets/g9.png");break;
			case 9:pe_y = addImage("assets/g10.png");break;
			default:PE_Y = 0;return;
		}
			PE_Y++;
			context.drawImage(pe_y,boardX,boardY-6);
	}
}
//结束动画
function The_Last_Effect(){
	if(life == 1){//赢了
		if(alp>=0&&alp<=1){
			if(alp_flag == 0){
				alp += 0.01;
			}
		}
		else{
			alp_flag = 1;
		}

		if(alp_flag == 1)
			alp -= 0.005;

		if(alp<0){
			alp = 0;
		}
		context.globalAlpha = alp;
		end = addImage("assets/victory.png");
		context.drawImage(end,0,0);	
		context.globalAlpha = alp;		
	}else if(life == 2){//死了
		alp += 0.003;
		if(alp>=1)
			alp = 1;
		end = addImage("assets/youdied.png");
		context.drawImage(end,0,0);
		context.beginPath();
		context.fillStyle='#000';
		context.globalAlpha = alp;
		context.fillRect(0,0, cW, cH);
		context.closePath();
	}
	context.globalAlpha = 1;
}