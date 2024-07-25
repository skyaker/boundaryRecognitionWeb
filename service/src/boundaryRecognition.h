#ifndef BOUNDARY_HPP
#define BOUNDARY_HPP

#include <opencv2/opencv.hpp>
#include <fstream>
#include "../include/json.hpp"
#include <string>

using cv::Mat;
using cv::imread;
using cv::imshow;
using cv::imwrite;
using cv::waitKey;
using std::cout;

/**
 * @brief Класс, содержащий основной и промежуточные методы обработки изображения для получения границ.
 */
class BoundaryRecognition {
public:
  //! Путь обрабатываемого изображения.
  const std::string& imagePath; 

  //! Индекс обрабатываемого изображения для случая, когда обрабатывается несколько изображений.
  size_t indexOfProcessibleImage;

  /**
   * @brief Конструктор, инициализирующий объект класса.
   * 
   * @param imagePath Путь обрабатываемого изображения.
   */
  BoundaryRecognition(const std::string& imagePath);

  /**
   * @brief Объединяющий все этапы обработки метод.
   * 
   * @return Изображение с задетектированными границами.
   */
  Mat getBoundaries();

private:
  //! Необработанное изображение.
  Mat originalImage; 

   //! Обрабатываемое изображение, с которым будут производиться операции.
  Mat processingImage;

  /**
   * @brief Наложение блюра для уменьшения влияния эффектов шума на результат.
   */
  void addAdaptiveBlur();

  /**
   * @brief Получение градиентов оператором Собеля.
   * 
   * @return Направление градиентов.
   */
  Mat findGradients();

  /**
   * @brief Подавляет немаксимальные значения градиентов по направлению для получения более тонких границ.
   * 
   * @param gradientDirections Полученные направления градиентов.
   */
  void suppressionOfNonMaximums(Mat& gradientDirections);

  /**
   * @brief Находит значение порога бинаризации методом Оцу.
   * 
   * @return Пороговое значение бинаризации.
   */
  int getOtsuThresh();

  /**
   * @brief Применение бинаризации по полученному пороговому значению.
   */
  void applyDoubleThreshold();

  /**
   * @brief Проверка корректности полученных границ по метрике F1.
   */
  void checkQuality();

  //! Доля правильных положительных предсказаний среди всех предсказанных положительных случаев.
  std::vector<double> precisionVec;

  //! Доля правильных положительных предсказаний среди всех фактических положительных случаев.
  std::vector<double> recallVec; 

  //! Значение точности по F1-метрике.
  std::vector<double> f1scoreVec; 

  //! Сохранение результата в формате json-файла.
  void writeResultsToJson();
};

#endif
