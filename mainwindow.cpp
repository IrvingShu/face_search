#include "mainwindow.h"
#include "ui_mainwindow.h"

const std::string IMGBASEPATH = "./data/img/";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->K = 10;
    ui->setupUi(this);
    //face detector file
    string shape_predictor_model = "./model/shape_predictor_68_face_landmarks.dat";
    //face recognization file
    string model_file = "./model/triplet_deploy.prototxt";
    string trained_file = "./model/_iter_175000.caffemodel";
    this->facedetector = new FaceDetector(shape_predictor_model);
    this->classifier = new Classifier(model_file, trained_file);

    this->gallery = new Gallery("./data/gallery_clusterfea.csv","./data/gallery_clusterlabel.csv",
                                    "./data/gallery_rangelist.csv","./data/gallery_imgpath.txt");
    this->probe = new Probe("./data/probe_label.csv");
    initWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(this->gallery){
        delete this->gallery;
        this->gallery = NULL;
    }
    if(this->facedetector){
        delete this->facedetector;
        this->facedetector = NULL;
    }
    if(this->classifier){
        delete this->classifier;
        this->classifier = NULL;
    }
}

void MainWindow::initWidget()
{
    connect(this->ui->resultlistWidget, SIGNAL(itemDoubleClicked ( QListWidgetItem *)), this, SLOT(changeImgSlot(QListWidgetItem *)));
    this->ui->imgNumLineEdit->setText(QString::number(this->gallery->getGalleryData().rows));
    this->ui->kLineEdit->setText(QString::number(K));
    this->ui->timeLineEdit->setText("0");
    this->ui->similarLineEdit->setText("0");
    this->setFixedSize(900,500);

}

void MainWindow::changeImgSlot(QListWidgetItem *item)
{
    int index = 0;
    for(unsigned int i=0; i<this->imgPaths.size(); i++)
    {
        if(item->text().toStdString() == imgPaths[i]){
            index = i;
            break;
        }
    }
    int num = this->distances.size();
    this->ui->similarLineEdit->setText(QString::number(this->distances[num-index-1]));

    std::string imgpath = IMGBASEPATH + item->text().toStdString();
    cv::Mat resultImg = cv::imread(imgpath);
    if(!resultImg.data){
        QMessageBox msgBox;
        msgBox.setText(tr("图片不存在!"));
        msgBox.exec();
    }else{
        cv::Mat galleryShowImg;
        cv::cvtColor(resultImg,galleryShowImg,CV_BGR2RGB);
        cv::resize(galleryShowImg, galleryShowImg, cv::Size(this->ui->imageresultLabel->width(), this->ui->imageresultLabel->height()), (0, 0), (0, 0), cv::INTER_LINEAR);
        QImage img = QImage((const unsigned char*)(galleryShowImg.data),galleryShowImg.cols,galleryShowImg.rows, galleryShowImg.cols*galleryShowImg.channels(),  QImage::Format_RGB888);
        this->ui->imageresultLabel->clear();
        this->ui->imageresultLabel->setPixmap(QPixmap::fromImage(img));
    }

}

void MainWindow::on_StartButton_clicked()
{
    std::vector<cv::Mat> imgResultMat = getSearchResult(this->testImgPath);
    //first search from gallerry
    this->imgPaths = getSearchImgPath(imgResultMat);
    //second search from gallery
    //std::vector<float> distances = reagainSearchImgPath(imgResultPath);
    //quickSortDistance(distances, imgResultPath);
    this->ui->resultlistWidget->clear();
    for(unsigned int i=0; i<this->imgPaths.size(); i++)
    {
        std::cout << this->imgPaths[i] << std::endl;
        QListWidgetItem* lst = new QListWidgetItem(QString::fromStdString(this->imgPaths[i]),this->ui->resultlistWidget);
        this->ui->resultlistWidget->insertItem(i+1,lst);
    }

    //show img
    cv::Mat resultImg = cv::imread(IMGBASEPATH + this->imgPaths[0]);
    if(!resultImg.data){
        QMessageBox msgBox;
        msgBox.setText(tr("图片不存在!"));
        msgBox.exec();
    }else{
        cv::Mat galleryShowImg;
        cv::cvtColor(resultImg,galleryShowImg,CV_BGR2RGB);
        cv::resize(galleryShowImg, galleryShowImg, cv::Size(this->ui->imageresultLabel->width(), this->ui->imageresultLabel->height()), (0, 0), (0, 0), cv::INTER_LINEAR);
        QImage img = QImage((const unsigned char*)(galleryShowImg.data),galleryShowImg.cols,galleryShowImg.rows, galleryShowImg.cols*galleryShowImg.channels(),  QImage::Format_RGB888);
        this->ui->imageresultLabel->clear();
        this->ui->imageresultLabel->setPixmap(QPixmap::fromImage(img));
        //set distance line edit
        int num = this->distances.size();
        this->ui->similarLineEdit->setText(QString::number(this->distances[num-1]));
    }
}

void MainWindow::on_OpenButton_clicked()
{
    QString imgname = QFileDialog::getOpenFileName(this,
                                                   tr("选择图像"),
                                                   "",
                                                  tr("Images (*.png *.bmp *.jpg *.tif *.GIF)")); //选择路径
    if(imgname.isEmpty())
    {
        return;
    }else{
        this->probeImg = cv::imread(imgname.toStdString());
        if(!probeImg.data)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Import image is failed!"));
            msgBox.exec();
        }else{
            cv::Mat probeShowImg;
            cv::cvtColor(probeImg,probeShowImg,CV_BGR2RGB);
            cv::resize(probeShowImg, probeShowImg, cv::Size(this->ui->imagelabel->width(), this->ui->imagelabel->height()), (0, 0), (0, 0), cv::INTER_LINEAR);
            QImage img = QImage((const unsigned char*)(probeShowImg.data),probeShowImg.cols,probeShowImg.rows, probeShowImg.cols*probeShowImg.channels(),  QImage::Format_RGB888);
            this->ui->imagelabel->clear();
            this->ui->imagelabel->setPixmap(QPixmap::fromImage(img));
            this->testImgPath = imgname.toStdString();
        }
    }
}

std::vector<float> MainWindow::getAllDistance(const std::vector<float> &feature1, const cv::Mat &galleryData)
{
    std::vector<float> allDistance;
    for(int i=0; i<galleryData.rows; ++i)
    {
        cv::Mat tempEachRowGarrery = galleryData.row(i);
        double distance = getEucDistance(feature1, tempEachRowGarrery);
        allDistance.push_back(distance);
    }
    return allDistance;
}

float MainWindow::getEucDistance(const std::vector<float> &feature1, const cv::Mat &galleryEachRow)
{
    float distance = 0.0;
    if(feature1.size() == galleryEachRow.cols)
    {
        const float *data = galleryEachRow.ptr<float>(0);
        for(unsigned int i=0; i < feature1.size(); i++)
        {

            distance += pow(feature1[i] - data[i],2);
        }
        distance = sqrt(distance);
    }else{
       std::cout << "feature1: "<< feature1.size()<< "feature2: " << galleryEachRow.cols << std::endl;
    }
    return distance;
}

void MainWindow::getTopKDistance(std::vector<float> &distances, std::vector<float> &labels ,int k)
{
    int n = distances.size();
    for(int i=0; i<k; ++i)
    {
        for(int j=1; j<n-i; j++)
        {
            if(distances[j-1] < distances[j])
            {
                //exchange data
                float temp = distances[j-1];
                distances[j-1] = distances[j];
                distances[j] = temp;
                //exchange label
                float tempLabel = labels[j-1];
                labels[j-1] = labels[j];
                labels[j] = tempLabel;
            }
        }
    }
}

float MainWindow::getEucDistance(const std::vector<float> &feature1, const std::vector<float> &feature2)
{
    float distance = 0.0;
    if(feature1.size() == feature2.size())
    {
        for(unsigned int i=0; i < feature1.size(); i++)
        {

            distance += pow(feature1[i] - feature2[i],2);
        }
        distance = sqrt(distance);
    }else{
       std::cout << "feature1: "<< feature1.size()<< "feature2: " << feature2.size() << std::endl;
    }
    return distance;

}

std::vector<cv::Mat> MainWindow::getSearchResult(std::string imgPath)
{
   std::vector<cv::Mat> imgFaceChip = this->facedetector->getFaceImg(imgPath);
   std::vector<float> imgFeature;
   //if only one face
   if(imgFaceChip.size() == 1)
   {
      cv::resize(imgFaceChip[0], imgFaceChip[0], cv::Size(300, 300), (0, 0), (0, 0), cv::INTER_LINEAR);
      std::cout << "resize" << std::endl;
      imgFeature = this->classifier->getFeature(imgFaceChip[0]);
   }
   else{
      cv::resize(this->probeImg, this->probeImg, cv::Size(300, 300), (0, 0), (0, 0), cv::INTER_LINEAR);
      std::cout << "resize" << std::endl;
      imgFeature = this->classifier->getFeature(this->probeImg);
   }
   //copy imgfeature to testImgFeature
   this->testImgFeature.assign(imgFeature.begin(), imgFeature.end());

   this->distances = getAllDistance(imgFeature, this->gallery->getGalleryData());
   std::vector<float> imglabels = this->gallery->getGalleryLabel();

   getTopKDistance(this->distances, imglabels, K);
   //save testImg top1 label
   if(this->K == 1){
       std::vector<cv::Mat> resultImg;
      testImgLabels.push_back(imglabels[0]);
      return resultImg;
   }else{
       std::vector<cv::Mat> resultImg;
       for(int i=0; i<K; i++)
       {
            float tempimglabel = imglabels.back();
            imglabels.pop_back();
            resultImg.push_back(this->gallery->getGalleryRangeList().row(tempimglabel-1));
       }
       return resultImg;
   }
}

std::vector<std::string> MainWindow::getSearchImgPath(const std::vector<cv::Mat> &imgResultMat)
{
    std::vector<std::string> imgpathset;
    srand((unsigned)time(NULL));//设置随机数种子
    for(unsigned int i=0; i<imgResultMat.size(); i++)
    {
        const float *data = imgResultMat[i].ptr<float>(0);
        int randImgIndex = (rand()%((int)data[1] - (int)data[0] + 1)) + (int)data[0];
        imgpathset.push_back(this->gallery->getGalleryImgSet()[randImgIndex]);
    }
    return imgpathset;
}

void MainWindow::quickSortDistance(std::vector<float> &distances, std::vector<std::string> &path)
{
    int n = distances.size();
    for(int i=0; i<n; ++i)
    {
        for(int j=1; j<n-i; j++)
        {
            if(distances[j-1] > distances[j])
            {
                //exchange data
                float temp = distances[j-1];
                distances[j-1] = distances[j];
                distances[j] = temp;
                //exchange label
                std::string tempPath = path[j-1];
                path[j-1] = path[j];
                path[j] = tempPath;
            }
        }
    }
}

//again search image
std::vector<float> MainWindow::reagainSearchImgPath(std::vector<std::string> imgPathSet)
{
    std::vector<float> distances;
    for(unsigned int i=0; i < imgPathSet.size(); i++)
    {
        cv::Mat galleryImg = cv::imread(IMGBASEPATH + imgPathSet[i]);
        std::vector<cv::Mat> imgFaceChip = this->facedetector->getFaceImg(IMGBASEPATH + imgPathSet[i]);
        std::vector<float> imgFeature;
        //if only one face
        if(imgFaceChip.size() == 1)
        {
           imgFeature = this->classifier->getFeature(imgFaceChip[0]);
        }
        else{
           imgFeature = this->classifier->getFeature(galleryImg);
        }
        float distance = getEucDistance(this->testImgFeature, imgFeature);
        distances.push_back(distance);
    }
    return distances;
}

void MainWindow::on_testLibraryButton_clicked()
{
    this->K = 1;
    QString filename = QFileDialog::getOpenFileName(this,
                                                   tr("选择文件"),
                                                   "",
                                                  tr("Files (*.txt)")); //选择路径
    if(filename.isEmpty())
    {
        std::cout << "img not exist!" << std::endl;
        return;
    }else{
        Utils utils;
        std::vector<std::string> probeimgpath = utils.loadTxt(filename.toStdString());
        for(int i=0; i<probeimgpath.size(); i++)
        {
            this->probeImg = cv::imread(IMGBASEPATH + probeimgpath[i]);
            std::vector<cv::Mat> imgResultMat = getSearchResult(IMGBASEPATH + probeimgpath[i]);
        }
        float result = getAllImgAuc(this->testImgLabels, this->probe->getProbeLabels());
        std::cout << result << std::endl;
        this->ui->searchRateLineEdit->setText(QString::number(result));
    }
}

float MainWindow::getAllImgAuc(std::vector<float> testLabels, std::vector<float> probeLabels)
{
    int all = probeLabels.size();
    int count = 0;
    if(testLabels.size() == probeLabels.size())
    {
        for(int i=0; i<probeLabels.size(); i++)
        {
            if(testLabels[i] == probeLabels[i])
            {
                count ++;
            }
        }
    }
    return 1.0 * count / all;
}
