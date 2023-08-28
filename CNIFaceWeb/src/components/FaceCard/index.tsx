import styles from './index.less';
import moment from 'moment';


export type FaceCardProps = {
  imageUri: string;
  name: string;
  personId: string;
  timestamp: number;
  score?: string;
};

const FaceCard: React.FC<FaceCardProps> = ({
  imageUri,
  name,
  personId,
  timestamp,
  score
}) => {

  return (
  <div
    className={styles.wrapper}
  >
    <img className={styles.img} src={imageUri} />
    <div className={styles.info}>
      <div className={styles.infoItem}>{`姓名：${name}`}</div>
      <div className={styles.infoItem}>{`ID：${personId}`}</div>
      <div className={styles.infoItem}>{`时间：${moment(timestamp).format('YYYY-MM-DD hh:mm:ss')}`}</div>
      {score && <div className={styles.infoItem}>{`分数：${score}`}</div>}
    </div>
    
  </div>)
}

export default FaceCard;