import javax.swing.*;
import java.awt.*;
import java.util.TimerTask;

public class GraphDemo extends JFrame {
    public static int TESTNUM = 80;
    public static int WIDTH=1500;
    public static int HEIGHT=900;
    public Execute execute;
    public Line[] lines;
    public int top;
    GraphDemo(){
        top=0;
        execute = new Execute();
        lines=new Line[execute.top];
        for(int i=0;i<execute.top;i++){
            lines[i] = execute.lines[i];
        }
        setSize(WIDTH,HEIGHT);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    public void paint(Graphics draw){
        Line cloestLine=new Line(new Point(0,0),new Point(600,1000));
        Line bufferLines[]=new Line[1000];
        int bufferTop=0;
        draw.setColor(Color.BLACK);
        while(top!=lines.length){
            for(int i=0;i<TESTNUM;i++){
                draw.setColor(Color.RED);
                draw.drawOval((int)execute.points[i].x,(int)execute.points[i].y,3,3);
                draw.fillOval((int)execute.points[i].x,(int)execute.points[i].y,3,3);

                draw.setColor(Color.BLUE);
                draw.setFont(new Font("宋体", Font.BOLD, 10));
                draw.drawString("["+ (int)execute.points[i].x + "," + (int)execute.points[i].y + "]",
                        (int)execute.points[i].x, (int)execute.points[i].y);
            }

            draw.setColor(Color.white);
            for(int i=0;i<bufferTop;i++) {
                if (bufferLines[i].dis != cloestLine.dis || bufferLines[i].xline) {
                    draw.drawLine((int) bufferLines[i].from.x, (int) bufferLines[i].from.y,
                            (int) bufferLines[i].to.x, (int) bufferLines[i].to.y);
                }
            }
            /*try
            {
                Thread.currentThread().sleep(100);//毫秒
            }
            catch(Exception e){}*/
            bufferTop=0;
            Line min = new Line(cloestLine.from,cloestLine.to);
            while(top!=lines.length){
                if(lines[top].xline){
                    draw.setColor(Color.GRAY);
                }else{
                    draw.setColor(Color.BLACK);
                }
                draw.drawLine((int)lines[top].from.x,(int)lines[top].from.y,
                        (int)lines[top].to.x,(int)lines[top].to.y);

                if(!lines[top].xline&&lines[top].dis<min.dis){
                    min=lines[top];
                }
                bufferLines[bufferTop++]=lines[top];
                //repaint();
                if(lines[top].endFlag){
                    top++;
                    break;
                }else{
                    top++;
                }
            }
            if(min.dis<cloestLine.dis) {
                draw.setColor(Color.white);
                draw.drawLine((int) cloestLine.from.x, (int) cloestLine.from.y,
                        (int) cloestLine.to.x, (int) cloestLine.to.y);
                cloestLine = min;
                draw.setColor(Color.RED);
                draw.drawLine((int) cloestLine.from.x, (int) cloestLine.from.y,
                        (int) cloestLine.to.x, (int) cloestLine.to.y);
            }else{
                draw.setColor(Color.RED);
                draw.drawLine((int) cloestLine.from.x, (int) cloestLine.from.y,
                        (int) cloestLine.to.x, (int) cloestLine.to.y);
            }
        }

        System.out.println("end");
    }
    public static void main(String[] agrs){
        new GraphDemo();
    }
}
