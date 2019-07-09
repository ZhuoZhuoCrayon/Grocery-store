public class Line {
    public Point from;
    public Point to;
    public double dis;
    public boolean endFlag;
    public boolean xline;
    Line(Point p1,Point p2){
        this.from=new Point(p1.x,p1.y);
        this.to=new Point(p2.x,p2.y);
        this.dis=p1.dis(p2);
        this.endFlag=false;
        this.xline=false;
    }
    public void end(){
        endFlag=true;
    }
    public void xline(){
        this.xline=true;
    }
}
