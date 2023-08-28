import { PageContainer } from '@ant-design/pro-components';
import { message, Button, Slider } from 'antd';
import { useCallback, useState } from 'react';
import { useIntl } from 'umi';
import { faceDetectAPI, extractFeatureAPI, similarityAPI } from '@/services/cniface/api';
import { cutImgBase64Prefix } from '@/utils';
import type { SelectRect } from './data';
import styles from './index.less';

import FaceDetector from '@/components/FaceDetector';

const FaceComparison: React.FC = () => {
  const intl = useIntl();

  const [similarityValue, setSimilarityValue] = useState("00.00");

  const [selectRects, setSelectRects] = useState([] as SelectRect[]);
  const [forSelectImgBase64, setForSelectImgBase64] = useState("");

  const [sourceImgBase64, setSourceImgBase64] = useState("");

  const [imgBase64_1, setImgBase64_1] = useState("");
  const [imgBase64_2, setImgBase64_2] = useState("");

  const [feature1, setFeature1] = useState([] as number[]);
  const [feature2, setFeature2] = useState([] as number[]);

  const [faceScoreThreshold, setFaceScoreThreshold] = useState(0.5);

  const faceDetectorCallback = useCallback(
    async (imgBase64, id) => {
      const response = await faceDetectAPI({
        faceImageBase64: imgBase64,
        score: faceScoreThreshold,
        isReturnRecognitionSubImage: false,
      });
      if (response.code !== 0) {
        console.error(response.message);
        return;
      }
      const { result } = response;
      if (result.length === 0) {
        message.warn(intl.formatMessage({
          id: 'face.detect.noFace',
          defaultMessage: '未检测到人脸',
        }))
        return;
      }
      const image = new Image();
      image.src = `data:image/png;base64,${imgBase64}`;
      image.crossOrigin = 'anonymous';
      image.onload = async () => {
        const canvas = document.createElement('canvas');
        const ctx = canvas.getContext('2d');
        if (!ctx) {
          console.error("ctx is null");
          return;
        };

        // 如果只有一个人脸直接显示
        if (result.length === 1) {
          const face = result[0];
          canvas.width = face.w;
          canvas.height = face.h;
          ctx.drawImage(image, face.x, face.y, face.w, face.h, 0, 0, face.w, face.h);
          const kp = face.kps;

          ctx.font = `bold ${face.w / 5}px sans-serif`;
          ctx.textAlign = 'center';
          ctx.fillStyle = 'red';
          ctx.fillText(face.score.toFixed(4).toString(), face.w / 2, face.w / 5);

          for (let i = 0; i < 10; i += 2) {
            ctx.fillStyle = 'red';
            ctx.fillRect(kp[i] - face.x, kp[i + 1] - face.y, 3, 3)
          }
          const imgUri = canvas.toDataURL('image/png');
          const extractFeatureResponse = await extractFeatureAPI({
            faceImageBase64: imgBase64,
            kps: face.kps
          });
          if (id == 1) {
            setFeature1(extractFeatureResponse.result.feature);
            setImgBase64_1(cutImgBase64Prefix(imgUri));
          }
          if (id == 2) {
            setFeature2(extractFeatureResponse.result.feature);
            setImgBase64_2(cutImgBase64Prefix(imgUri));
          }
          return;
        }

        // 如果多张人脸，框出来，供用户选择。
        const tempSelectRects = [];
        canvas.width = image.width;
        canvas.height = image.height;
        ctx.drawImage(image, 0, 0, image.width, image.height);
        for (let i = 0; i < result.length; i++) {
          const face = result[i];

          const x_scale = image.width / 960;
          const y_scale = image.height / 540;
          tempSelectRects.push({
            id: i,
            x: face.x / x_scale,
            y: face.y / y_scale,
            w: face.w / x_scale,
            h: face.h / y_scale,
            face: face,
            detectorId: id,
          });


          ctx.font = `bold ${face.w / 5}px sans-serif`;
          ctx.textAlign = 'center';
          ctx.fillStyle = 'red';
          ctx.fillText(face.score.toFixed(4).toString(), face.x + (face.w / 2), face.y + (face.w / 5));

          ctx.lineWidth = 1;
          ctx.strokeStyle = "green";
          ctx.rect(face.x, face.y, face.w, face.h);
          ctx.stroke();

          const kp = face.kps;
          ctx.fillStyle = 'red';
          for (let k = 0; k < 10; k += 2) {
            ctx.fillRect(kp[k], kp[k + 1], 3, 3)
          }
        }
        const imgUri = canvas.toDataURL('image/png');
        setSourceImgBase64(imgBase64);
        setSelectRects(tempSelectRects);
        setForSelectImgBase64(cutImgBase64Prefix(imgUri));
      }
    },
    [faceScoreThreshold, intl]
  )

  const onSelectRectClick = useCallback(
    async (selectRect) => {
      const { face, detectorId } = selectRect;
      const canvas = document.createElement('canvas');
      const ctx = canvas.getContext('2d');
      if (!ctx) {
        console.error("ctx is null");
        return;
      };
      const image = new Image();
      image.src = `data:image/png;base64,${sourceImgBase64}`;
      image.crossOrigin = 'anonymous';
      image.onload = async () => {
        canvas.width = face.w;
        canvas.height = face.h;
        ctx.drawImage(image, face.x, face.y, face.w, face.h, 0, 0, face.w, face.h);
  
        ctx.font = `bold ${face.w / 5}px sans-serif`;
        ctx.textAlign = 'center';
        ctx.fillStyle = 'red';
        ctx.fillText(face.score.toFixed(4).toString(), face.w / 2, face.w / 5);
  
        const kp = face.kps;
        for (let i = 0; i < 10; i += 2) {
          ctx.fillStyle = 'red';
          ctx.fillRect(kp[i] - face.x, kp[i + 1] - face.y, 3, 3)
        }
        const imgUri = canvas.toDataURL('image/png');
        const extractFeatureResponse = await extractFeatureAPI({
          faceImageBase64: sourceImgBase64,
          kps: face.kps
        });
        if (detectorId == 1) {
          setFeature1(extractFeatureResponse.result.feature);
          setImgBase64_1(cutImgBase64Prefix(imgUri));
        }
        if (detectorId == 2) {
          setFeature2(extractFeatureResponse.result.feature);
          setImgBase64_2(cutImgBase64Prefix(imgUri));
        }
        setSelectRects([]);
        setForSelectImgBase64("");
      }
    },
    [sourceImgBase64]
  )

  const doRecognition = useCallback(
    async () => {
      const response = await similarityAPI({
        feature1,
        feature2,
      });
      if (response.code !== 0 ) {
        message.error(response.message);
        return;
      }
      setSimilarityValue(response.result.similarity.toFixed(2).toString().padStart(5, '0'));
    },
    [feature1, feature2]
  )

  const onFaceScoreThresholdSliderChange = useCallback(
    (newValue: number) => {
      setFaceScoreThreshold(newValue);
    },
    []
  )

  return (
    <PageContainer>
      <div className={styles.wrapper}>
        <div className={styles.faceDetectorWrapper}>
          <div className={styles.selectImgWrapper} style={{display: selectRects.length ? 'inline' : 'none'}}>
            {selectRects.map((selectRect) => 
              <div 
                key={selectRect.id}
                className={styles.selectRect}  
                style={{
                  left: selectRect.x,
                  top: selectRect.y,
                  width: selectRect.w,
                  height: selectRect.h,
                }}
                onClick={() => onSelectRectClick(selectRect)}
              />
            )}
            <img src={`data:image/png;base64,${forSelectImgBase64}`}/>
          </div>
          <FaceDetector
            imgBase64={imgBase64_1}
            base64CallBack={(base64) => {
              faceDetectorCallback(base64, 1)
            }}
          />
          <div className={styles.similarityWrapper}>
            <div className={styles.similarityValue}>
              {similarityValue}
            </div>
            <Button 
              type='ghost'
              shape='round'
              size='large'
              disabled={feature1.length === 0 || feature2.length === 0}
              onClick={doRecognition}
            >{intl.formatMessage({
                id: 'pages.face_comparison.comparison_button',
                defaultMessage: '比对',
              })}
            </Button>
          </div>
          <FaceDetector 
            imgBase64={imgBase64_2}
            base64CallBack={(base64) => {
              faceDetectorCallback(base64, 2)
            }}
          />
        </div>
      </div>
      <div className={styles.faceScoreThresholdSliderWrapper}>
        <Slider 
          className={styles.faceScoreThresholdSlider}
          min={0.1}
          max={0.9}
          step={0.1}
          value={faceScoreThreshold}
          onChange={onFaceScoreThresholdSliderChange}
          tipFormatter={(value) => `${intl.formatMessage({
            id: 'pages.face_comparison.faceScoreThresholdSliderTipPrefix',
            defaultMessage: '人脸检测可置信阈值：',
          })} ${value}`.toString()}
        />
      </div>
    </PageContainer>
  );
};

export default FaceComparison;
