import { useEffect, useState, useCallback } from 'react';
import { history, useIntl } from 'umi';
import { message, Input, Button } from 'antd';
import { queryRepositoryItemAPI, retrievalRepositoryItemAPI, SCENE_IMAGE_URL } from '@/services/cniface/api';
import FaceDetector from '@/components/FaceDetector';
import FaceCard from '@/components/FaceCard';
import styles from './index.less';

type FaceItem = {
  id: number;
  imageUri: string;
  name: string;
  personId: string;
  timestamp: number;
  score?: number;
}

const RepoItem: React.FC = () => {
  const intl = useIntl();

  const [pagination, setPagination] = useState({page: 1, size: 20});
  const [imgBase64, setImgBase64] = useState("");
  const [topk, setTopk] = useState(100);
  const [name, setName] = useState("");
  const [personId, setPersonId] = useState("");
  const [faceList, setFaceList] = useState([] as FaceItem[]);

  const currentRepoId = useCallback(() => {
    const query = history.location.query;
    if (!query?.id) {
      message.warn(intl.formatMessage({
        id: 'pages.repo_item.noRepoIdWarn',
        defaultMessage: '人像库id是必须的在url中。 例子: ?id=1',
      }));
      throw new Error(intl.formatMessage({
        id: 'pages.repo_item.noRepoIdWarn',
        defaultMessage: '人像库id是必须的在url中。 例子: ?id=1',
      }));
    }
    return parseInt(query?.id as string);
  }, [intl])

  const queryRepositoryItem = useCallback( async () => {
    const queryParam = {
      repoId: currentRepoId(),
      page: pagination.page,
      size: pagination.size,
      name,
      personId,
    }
    const response = await queryRepositoryItemAPI(queryParam);
    if (response.code !== 0) {
      console.error(response.message);
      message.error(response.message);
      return;
    }
    const { result } = response;
    const tempFaceList = [] as FaceItem[];
    for (const item of result) {
      tempFaceList.push({
        id: item.id,
        imageUri: SCENE_IMAGE_URL(currentRepoId(), item.id),
        name: item.name,
        personId: item.personId,
        timestamp: item.timestamp,
      });
    }
    setFaceList(tempFaceList);
  }, [currentRepoId, name, pagination.page, pagination.size, personId])

  useEffect(() => {
    queryRepositoryItem();
  }, [queryRepositoryItem])

  const imageSearchCallback = useCallback(
    async (_imgBase64: string) => {
      const response = await retrievalRepositoryItemAPI({
        repoId: currentRepoId(),
        topk,
        imageBase64: _imgBase64,
        detectScore: 0.5,
      })
      if (response.code !== 0) {
        console.error(response.message);
        message.error(response.message);
        return;
      }
      const { result, total } = response;
      console.log(result);
      const tempFaceList = [] as FaceItem[];
      for (const item of result) {
        tempFaceList.push({
          id: item.id,
          imageUri: SCENE_IMAGE_URL(currentRepoId(), item.id),
          name: item.name,
          personId: item.personId,
          timestamp: item.timestamp,
          score: item.score,
        });
      }
      setFaceList(tempFaceList);
    },
    [currentRepoId, topk]
  )
  
  return (
    <>
      <div className={styles.wrapper}>
        <div className={styles.plane}>
          <div
            className={styles.infoItem}
          >
            <FaceDetector
              width='250px'
              height='250px'
              imgBase64={imgBase64}
              base64CallBack={(base64) => {
                imageSearchCallback(base64)
              }}
            />
          </div>
          {/* <Input
            className={styles.infoItem}
            placeholder={
            intl.formatMessage({
              id: 's',
              defaultMessage: '姓名',
            })
            }
            // onChange={onChangeCeateRepositoryModelNameInput}
            // value={ceateRepositoryModelNameInput}
          />
          <Input
            className={styles.infoItem}
            placeholder={
            intl.formatMessage({
              id: 's',
              defaultMessage: 'ID',
            })
            }
            // onChange={onChangeCeateRepositoryModelNameInput}
            // value={ceateRepositoryModelNameInput}
          /> */}
          {/* <Button>检索</Button> */}
        </div>
        <div className={styles.faceList}>
            {faceList.map(faceItem => 
              <FaceCard
                key={faceItem.id}
                imageUri={faceItem.imageUri}
                name={faceItem.name}
                personId={faceItem.personId}
                timestamp={faceItem.timestamp}
                score={faceItem.score?.toString()}
              />
            )}
        </div>
      </div>
    </>
  )
}

export default RepoItem;