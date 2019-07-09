import java.util.Comparator;

class Point {
    public double x;
    public double y;
    Point(double x,double y){
        this.x =x;
        this.y=y;
    }
    double dis(Point point){
        return Math.sqrt((this.x-point.x)*(this.x-point.x)+(this.y-point.y)*(this.y-point.y));
    }
}

class sort_Y implements Comparator<Point>{
    @Override
    public int compare(Point p1,Point p2){
        if(p1.y<p2.y){
            return -1;
        }else if(p1.y>p2.y){
            return 1;
        }else{
            return 0;
        }
    }
}

class sort_X implements Comparator<Point>{
    @Override
    public int compare(Point p1,Point p2){
        if(p1.x<p2.x){
            return -1;
        }else if(p1.x>p2.x){
            return 1;
        }else{
            return 0;
        }
    }
}

