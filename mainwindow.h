#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QListWidgetItem>
#include <stdlib.h>
#include <time.h>

#include "gallery.h"
#include "probe.h"
#include "facedetector.h"
#include "classifier.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_StartButton_clicked();

    void on_OpenButton_clicked();
    void changeImgSlot(QListWidgetItem *item);

    void on_testLibraryButton_clicked();

private:
    Ui::MainWindow *ui;

    Classifier *classifier;
    FaceDetector *facedetector;
    Gallery *gallery;
    Probe *probe;
    cv::Mat probeImg;
    std::string testImgPath;
    std::vector<float> distances;
    std::vector<std::string> imgPaths;
    std::vector<float> testImgFeature;
    std::vector<float> testImgLabels;
    int K;

    std::vector<cv::Mat> getSearchResult(std::string imgPath);
    float getEucDistance(const std::vector<float> &feature1, const cv::Mat &galleryEachRow);
    float getEucDistance(const std::vector<float> &feature1, const std::vector<float> &feature2);
    std::vector<float> getAllDistance(const std::vector<float> &feature1, const cv::Mat &galleryData);
    void getTopKDistance(std::vector<float> &distances, std::vector<float> &labels, int k);
    std::vector<std::string> getSearchImgPath(const std::vector<cv::Mat> &imgResultMat);
    std::vector<float> reagainSearchImgPath(std::vector<std::string> imgPathSet);

    void quickSortDistance(std::vector<float> &distances, std::vector<std::string> &path);
    float getAllImgAuc(std::vector<float> testLabels, std::vector<float> probeLabels);


    void initWidget();
};

#endif // MAINWINDOW_H
