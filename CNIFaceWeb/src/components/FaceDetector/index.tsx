import { useCallback, useRef } from 'react'
import styles from './index.less';

export type FaceDetectorProps = {
  imgBase64?: string;
  width?: string;
  height?: string;
  base64CallBack?: (base64: string) => void;
};

const FaceDetector: React.FC<FaceDetectorProps> = ({
  imgBase64,
  width,
  height,
  base64CallBack = () => {},
}) => {
  const inputFile = useRef<HTMLInputElement>(null);

  const uploadImg = useCallback(
    (file: File) => {
      new Promise(resolve => {
        if (!(file instanceof File)) {
          return;
        }
        const fileReader = new FileReader();
        fileReader.readAsDataURL(file);
        fileReader.onload = () => {
          resolve(fileReader.result);
        }
        fileReader.onerror = () => {
          console.error("read file failed!");
          resolve("");
        }
      }).then((imgUri: any) => {
        const _imgBase64 = imgUri.replace(/^data:image\/\w+;base64,/, "");
        base64CallBack(_imgBase64);
      })
    },
    [base64CallBack]
  )

  const onClick = useCallback(
    (e: React.MouseEvent<HTMLDivElement> | React.KeyboardEvent<HTMLDivElement>) => {
      e.stopPropagation();
      if (!inputFile.current) {
        return;
      }
      inputFile.current.click();
    },
    []
  )

  const onInputFileChange = useCallback(
    (e: React.ChangeEvent<HTMLInputElement>) => {
      // @ts-ignore
      let files = [...e.target.files]; 
      files = files.slice(0, 1);
      uploadImg(files[0]);
      e.target.value = '';
    },
    [uploadImg]
  )


  const onFileDrop = useCallback(
    (e: React.DragEvent<HTMLDivElement>) => {
      e.preventDefault();
      if (e.type === 'dragover') {
        return;
      }
      // @ts-ignore
      let files = [...e.dataTransfer.files];
      files = files.slice(0, 1);
      uploadImg(files[0]);
    },
    [uploadImg]
  )

  return (
  <div
    className={styles.wrapper}
    style={{width, height}}
    role="button"
    onDrop={onFileDrop}
    onDragOver={onFileDrop}
    onClick={onClick}
  >
    <input 
      ref={inputFile} 
      onChange={onInputFileChange} 
      onClick={e => e.stopPropagation()}
      accept="image/jpeg,image/jpg,image/png" 
      style={{ display: 'none' }} 
      multiple={false}
      type="file" 
    />
    <img 
      className={styles.img} src={imgBase64 ? `data:image/png;base64,${imgBase64}` : '/img/default-avator-128x128.png'} 
    />
    
  </div>)
}

export default FaceDetector;