import java.util.Arrays;
import java.util.Comparator;
import java.util.Random;

public class Execute {
    public static int TESTNUM = 80;
    public static int WIDTH=1500;
    public static int HEIGHT=900;
    public Point[] points;
    public Point[] tmps;
    public Line[] lines;
    public int top;
    Execute(){
        tmps=new Point[TESTNUM];
        points=new Point[TESTNUM];
        Random r = new Random();
        for(int i=0;i<TESTNUM;i++){
            //points[i]=new Point(r.nextDouble()*WIDTH,r.nextDouble()*HEIGHT);
            points[i]=new Point((double) r.nextInt(WIDTH-200)+50,(double)r.nextInt(HEIGHT-100)+50);
        }
        lines=new Line[100*TESTNUM];
        top=0;
        Comparator cmp = new sort_X();
        Arrays.sort(points,cmp);
        for(int i=0;i<TESTNUM;i++){
            System.out.println(points[i].x);
        }
        System.out.println(divide(0,TESTNUM-1));
    }
    double cal_distace(int left,int right){
        double leftBounds,right_Bounds;
        leftBounds=0;
        right_Bounds=WIDTH;
        if(left!=0){
            leftBounds = (points[left].x+points[left-1].x)/2;
        }
        if(right!=TESTNUM-1){
            right_Bounds = (points[right].x+points[right+1].x)/2;
        }
        lines[top++]=new Line(new Point(leftBounds,0),new Point(leftBounds,HEIGHT));
        lines[top-1].xline();
        lines[top++]=new Line(new Point(right_Bounds,0),new Point(right_Bounds,HEIGHT));
        lines[top-1].xline();

        int cnt=right-left+1;
        if(cnt==1){
            lines[top++]=new Line(points[left],points[left]);
            lines[top-1].end();
            return Double.MAX_VALUE;
        }else if(cnt==2) {
            lines[top++] = new Line(points[left], points[right]);
            lines[top-1].end();
            return lines[top - 1].dis;
        }else{
            lines[top++]=new Line(points[left],points[right]);
            lines[top++]=new Line(points[left+1],points[right]);
            lines[top++]=new Line(points[left],points[left+1]);
            lines[top-1].end();
            return Double.min(Double.min(lines[top-1].dis,lines[top-1].dis),lines[top-3].dis);
        }
    }
    double merge(int left,int mid,int right,double min_dis){
        /*double down = points[mid].x-min_dis;
        double up=points[mid].x+min_dis;
        int l,r;
        for(l=mid;l>=left&&points[l].x>=down;l--);
        l++;
        for(r=mid+1;r<=right&&points[r].x<=up;r++);
        r--;*/
        int index=0,bound;
        for(int i=mid;i>=left&&points[mid].x-points[i].x<=min_dis;i--){
            tmps[index++]=new Point(points[i].x,points[i].y);
        }
        bound=index;

        for(int i=mid+1;i<=right&&points[i].x-points[mid].y<=min_dis;i++){
            tmps[index++]=new Point(points[i].x,points[i].y);
        }
        lines[top]=new Line(new Point(points[mid].x-min_dis,0),new Point(points[mid].x-min_dis,HEIGHT));
        lines[top++].xline();
        lines[top]=new Line(new Point(points[mid].x,0),new Point(points[mid].x,HEIGHT));
        lines[top++].xline();
        lines[top]=new Line(new Point(points[mid].x+min_dis,0),new Point(points[mid].x+min_dis,HEIGHT));
        lines[top++].xline();


        Comparator cmp=new sort_Y();
        Arrays.sort(tmps,bound,index,cmp);
        for(int i=0;i<bound;i++){
            double north=tmps[i].y+min_dis;
            double south=tmps[i].y-min_dis;
            double east=tmps[i].x+min_dis;

            lines[top]=new Line(new Point(tmps[i].x,north),new Point(tmps[i].x,south));
            lines[top++].xline();
            lines[top]=new Line(new Point(east,north),new Point(east,south));
            lines[top++].xline();
            lines[top]=new Line(new Point(tmps[i].x,north),new Point(east,north));
            lines[top++].xline();
            lines[top]=new Line(new Point(tmps[i].x,south),new Point(east,south));
            lines[top++].xline();

            for(int j=bound;j<index;j++){
                if(tmps[j].y>north){
                    break;
                }
                if(tmps[j].y<=north&&tmps[j].y>=south&&tmps[j].x<=east){
                    lines[top++]=new Line(tmps[i],tmps[j]);
                    double dis = lines[top-1].dis;
                    min_dis = Double.min(min_dis,dis);
                }
            }
        }
        lines[top-1].end();
        return min_dis;
    }

    double divide(int left,int right){
        if(right-left+1<=3){
            return cal_distace(left,right);
        }else{
            int mid=(left+right)/2;
            double left_minDis=divide(left,mid);
            double right_minDis=divide(mid+1,right);
            double min_dis=Double.min(left_minDis,right_minDis);
            return merge(left,mid,right,min_dis);
        }
    }
    /*public static void main(String args[]){
        Execute test=new Execute();
        System.out.println(test.top);
    }*/
}
