#ifndef PAINTWIDGE_H
#define PAINTWIDGE_H

#include <QWidget>
#include<QMouseEvent>
#include<QPoint>

class Paintwidge : public QWidget{
    Q_OBJECT
public:
    //explicit Paintwidge(QWidget *parent = 0);
    Paintwidge();

public:
    enum ShapeType{
        None                     = 0,
        Line                        = 1,
        Rectangle            = 2,
        Ellipse                   = 3,
        ERASE                   = 4
    };

public:
    void setImageSize(int width, int height);
    void setImageColor(QColor color);
    bool isModified() const{return modified;}
    bool saveImage(const QString&fileName, const char*fileFormat);
    bool openImage(const QString&fileName);

    QSize getImageSize();
    const QImage& getImage(){
        return image;
    }

   // void doPrint();

    //! tool function
    void zoomIn();                                    //! magnify
    void zoomOut();                                //! narrow
    void zoomOrigin();                           //! return to original shape
    void doRotate();                                //! rotate
    void doShear();                                   //! stretch
    void doClear();

    //! pen modify
    Qt::PenStyle setPenStyle(Qt::PenStyle style);
             int           setPenWidth(int width);
         QColor      setPenColor(QColor color);
         QColor      setBrushColor(QColor color);
    ShapeType   setShape(ShapeType shape);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void paint(QImage&theImage);

signals:

public slots:


private:
    bool modified;                                              //! is modified
    QImage image;                                             //! paint object
    QRgb backColor;
    QPoint lastPoint, endPoint;

    //! tools
    qreal scale;
    int angle;
    qreal shear;

    //! pen
    QColor                   penColor;
    QColor                   brushColor;
        int                       penWidth;
    Qt::PenStyle        penStyle;
    ShapeType           curShape;


    QImage                 tempImage;             //! temporary painting district
    bool                       isDrawing;                //! is special feature
};

#endif // PAINTWIDGE_H
