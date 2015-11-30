#include "paintwidge.h"
#include<QPainter>
#include<QtPrintSupport/QPrintDialog>
#include<QtPrintSupport/QPrinter>

Paintwidge::Paintwidge(){
    image = QImage(400, 300, QImage::Format_RGB32);
    backColor = qRgb(255, 255, 255);
    image.fill(backColor);

    modified = false;
    scale = 1;
    angle = 0;
    shear = 0;

    penColor = Qt::black;
    brushColor = Qt::black;
    penWidth = 1;
    penStyle = Qt::SolidLine;
    curShape = None;
    isDrawing = false;
}

void Paintwidge::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.scale(scale, scale);
    if(isDrawing)
        painter.drawImage(0, 0, tempImage);
    else{
        if(angle){
            QImage copyImage = image;
            QPainter pp(&copyImage);
            QPointF center(copyImage.width()/2.0, copyImage.height()/2.0);
            pp.translate(center);
            pp.rotate(angle);
            pp.translate(-center);
            pp.drawImage(0, 0, image);
            image = copyImage;
            angle = 0;
        }
        if(shear){
            QImage copyImage = image;
            QPainter pp(&copyImage);
            pp.shear(shear, shear);
            pp.drawImage(0, 0, image);
            image = copyImage;
            shear = 0;
        }
        painter.drawImage(0, 0, image);
    }
}

QSize Paintwidge::getImageSize(){
    return image.size()*scale;
}

void Paintwidge::mousePressEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
        isDrawing = true;
    }
}

void Paintwidge::mouseMoveEvent(QMouseEvent * event){
    if(event->buttons() & Qt::LeftButton){
        endPoint = event->pos();

        if(curShape == None || curShape == ERASE){
            isDrawing = false;
            paint(image);
        }
        else{
            tempImage = image;
            paint(tempImage);
        }
    }
}

void Paintwidge::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        endPoint = event->pos();
        isDrawing = false;
        paint(image);
    }
}

void Paintwidge::paint(QImage &theImage){
    QPainter pp(&theImage);
    QPen pen = QPen();
    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    QBrush brush = QBrush(brushColor);
    pp.setPen(pen);
    pp.setBrush(brush);

    int x(0), y(0), w(0), h(0);
    x = lastPoint.x() / scale;
    y = lastPoint.y() / scale;
    w = (endPoint.x() - x) / scale;
    h = (endPoint.y() - y) / scale;

    switch(curShape){
    case None:
        pp.drawLine(lastPoint / scale, endPoint / scale);
        lastPoint = endPoint;
        break;
    case Line:
        pp.drawLine(lastPoint / scale, endPoint / scale);
        break;
    case Rectangle:
        pp.drawRect(x, y, w, h);
        break;
    case Ellipse:
        pp.drawEllipse(x, y, w, h);
        break;
    case ERASE:
        QRect rect(lastPoint, QPoint(lastPoint.x() + 10, lastPoint.y() + 10));
        pp.eraseRect(rect);
        lastPoint = endPoint;
        break;
    }
    update();
    modified = true;
}

void Paintwidge::setImageSize(int width, int height){
    QImage newImage(width, height, QImage::Format_RGB32);
    image =newImage;
    update();
}

void Paintwidge::setImageColor(QColor color){
    backColor = color.rgb();
    image.fill(backColor);
    update();
}

bool Paintwidge::saveImage(const QString &fileName, const char *fileFormat){
    QImage visibleImage = image;
    if(visibleImage.save(fileName, fileFormat)){
        modified = false;
        return true;
    }
    else
        return false;
}

bool Paintwidge::openImage(const QString &fileName){
    QImage loadedImage;
    if(!loadedImage.load(fileName))
        return false;
    QSize newSize = loadedImage.size();
    setImageSize(newSize.width(), newSize.height());
    image = loadedImage;
    update();
    return true;
}

/*void Paintwidge::doPrint(){
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog* printDialog = new QPrintDialog(&printer, this);
    if(printDialog->exec() == QDialog::Accepted){
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
}*/

void Paintwidge::zoomIn(){
    scale *= 1.2;
    update();
}

void Paintwidge::zoomOut(){
    scale /= 1.2;
    update();
}

void Paintwidge::zoomOrigin(){
    scale = 1;
    update();
}

void Paintwidge::doRotate(){
    angle += 90;
    update();
}

void Paintwidge::doShear(){
    shear = 0.2;
    update();
}

void Paintwidge::doClear(){
    image.fill(backColor);
    update();
}


Qt::PenStyle Paintwidge::setPenStyle(Qt::PenStyle style){
    Qt::PenStyle oldStyle = penStyle;
    penStyle = style;
    return oldStyle;
}

int Paintwidge::setPenWidth(int width){
    int oldWidth = penWidth;
    penWidth = width;
    return oldWidth;
}

QColor Paintwidge::setPenColor(QColor color){
    QColor oldColor = penColor;
    penColor = color;
    return oldColor;
}

QColor Paintwidge::setBrushColor(QColor color){
    QColor oldColor = brushColor;
    brushColor = color;
    return oldColor;
}

Paintwidge::ShapeType Paintwidge::setShape(ShapeType shape){
    ShapeType oldType = shape;
    curShape = shape;
    return oldType;
}
