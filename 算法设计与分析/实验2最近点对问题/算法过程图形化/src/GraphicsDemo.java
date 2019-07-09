import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;

import javax.swing.JFrame;

/**
 * 使用Graphics类绘图
 *
 * @author 小明
 *
 */
class GraphicsDemo extends JFrame {

    public GraphicsDemo() {
        setSize(1000, 600);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    @Override
    public void paint(Graphics g) {
        g.setColor(Color.RED);
        // 画线段
        g.drawLine(5, 5, 20, 100);
        // 画点
        g.drawLine(20, 20, 20, 20);

        // 画普通矩形框
        g.drawRect(30, 5, 100, 100);
        // 填充普通矩形
        g.fillRect(140, 5, 100, 100);

        // 画圆角矩形
        g.drawRoundRect(250, 5, 100, 100, 30, 30);
        // 填充圆角矩形
        g.fillRoundRect(360, 5, 100, 100, 40, 40);

        // 画三维矩形
        g.draw3DRect(5, 110, 100, 100, false);
        // 填充三维矩形
        g.fill3DRect(110, 110, 100, 100, true);

        // 画椭圆形
        g.drawOval(220, 110, 100, 50);
        // 填充椭圆形
        g.fillOval(330, 110, 30, 90);

        // 画圆弧
        g.drawArc(5, 220, 100, 100, 30, 150);
        // 填充圆弧
        g.fillArc(110, 220, 100, 100, 70, 220);

        // 画多边形
        int px[] = { 210, 220, 270, 250, 240 };
        int py[] = { 220, 250, 300, 270, 220 };
        g.drawPolygon(px, py, px.length);
        // 填充多边形
        int px1[] = { 310, 320, 370, 400, 340 };
        int py1[] = { 220, 250, 300, 270, 220 };
        g.fillPolygon(px1, py1, px.length);

        // 擦除块
        g.setColor(Color.green);
        g.fillOval(5, 330, 100, 100);
        g.clearRect(30, 350, 30, 60);

        // 限定图形显示区域
        g.clipRect(130, 380, 60, 60);
        g.clipRect(150, 400, 50, 50);
        g.fillRect(110, 330, 100, 100);
        g.setClip(null);

        // 绘制字符串
        g.setColor(Color.GREEN);
        g.setFont(new Font("楷体", Font.BOLD, 20));
        g.drawString("使用画笔绘制的字符串内容", 220, 345);

        // 绘制图像
        /*Image img = Toolkit.getDefaultToolkit().getImage("img/monster.gif");
        g.drawImage(img, 510, 5, 200, 200, Color.LIGHT_GRAY, this);*/

        // 复制图形
        //g.copyArea(0, 0, 500, 500, 505, 205);
    }

    public static void main(String[] args) {
        new GraphicsDemo();
    }
}
