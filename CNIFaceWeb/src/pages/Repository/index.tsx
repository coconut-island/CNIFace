import { PageContainer } from '@ant-design/pro-components';
import { message, Button, Table, Modal, Input, Popconfirm, Upload } from 'antd';
import { UploadOutlined } from '@ant-design/icons';
import type { RcFile, UploadProps } from 'antd/es/upload/interface';
import type { ColumnsType } from 'antd/es/table';
import { useCallback, useEffect, useState } from 'react';
import { useIntl, history } from 'umi';
import { 
  listRepositoryAPI, 
  createRepositoryAPI,
  deleteRepositoryAPI,
  insertRepositoryItemAPI,
} from '@/services/cniface/api';
import moment from 'moment';
import styles from './index.less';

interface RepositoryDataType {
  key: React.Key;
  id: number;
  name: string;
  total: number;
  createTime: string;
}

const Repository: React.FC = () => {
  const intl = useIntl();

  const [repositories, setRepositories] = useState([] as RepositoryDataType[]);

  const [isCreateRepositoryModalVisible, setIsCreateRepositoryModalVisible] = useState(false);
  const [ceateRepositoryModalNameInput, setCeateRepositoryModalNameInput] = useState("");
  const [createRepositoryModalOkLoading, setCreateRepositoryModalOkLoading] = useState(false);

  const [uploadRepositoryModalOkLoading, setUploadRepositoryModalOkLoading] = useState(false);
  const [uploadingRepo, setUploadingRepo] = useState(null as RepositoryDataType | null);
  const [uploadingImage, setUploadingImage] = useState("");
  const [uploadingName, setUploadingName] = useState("");
  const [uploadingPersonId, setUploadingPersonId] = useState("");
  const [uploadedImages, setUploadedImages] = useState([] as RcFile[]);
  

  const getRepositories = useCallback(
    async () => {
      const response = await listRepositoryAPI();
      if (response.code !== 0) {
        console.error(response.message);
        message.error(response.message);
        return;
      }

      const repsotiries = [] as RepositoryDataType[];
      
      for (const item of response.result) {
        repsotiries.push({
          key: item.id.toString(),
          id: item.id,
          name: item.name,
          total: item.total,
          createTime: moment(item.createTime).format('YYYY-MM-DD hh:mm:ss'),
        });
      }

      setRepositories(repsotiries);
    }
    ,[]
  )

  useEffect(
    () => {
      getRepositories();
    }
    ,[getRepositories]
  )

  const onClickDeletRepository = useCallback(
    async (row: RepositoryDataType) => {
      const response = await deleteRepositoryAPI({repoId: row.id});
      if (response.code === 0) {
        message.success(intl.formatMessage({
          id: 'pages.repository.deleteRepository.success',
          defaultMessage: '删除人像库成功',
        }));
      } else {
        message.error(response.message);
      }
      getRepositories();
    },
    [getRepositories, intl]
  )

  const handleCreateRepositoryModalOk = useCallback(
    async () => {
      setCreateRepositoryModalOkLoading(true);
      try {
        const response = await createRepositoryAPI({
          name: ceateRepositoryModalNameInput,
        })
        if (response.code === 0) {
          message.success(intl.formatMessage({
            id: 'pages.repository.createRepository.success',
            defaultMessage: '创建人像库成功',
          }));
        } else {
          message.error(response.message);
        }
        setIsCreateRepositoryModalVisible(false);
        getRepositories();
        setCreateRepositoryModalOkLoading(false);
      } catch (e) {
        setCreateRepositoryModalOkLoading(false);
      }
    },
    [ceateRepositoryModalNameInput, getRepositories, intl]
  )

  const handleCreateRepositoryModalCancel = useCallback(
    () => {
      setIsCreateRepositoryModalVisible(false);
    },
    []
  )

  const onClickCreateRpositoryButton = useCallback(
    () => {
      setCeateRepositoryModalNameInput("");
      setIsCreateRepositoryModalVisible(true);
    },
    []
  )

  const onChangeCeateRepositoryModalNameInput = useCallback(
    (e) => {
      setCeateRepositoryModalNameInput(e.target.value);
    },
    []
  )

  const openUploadRepositoryModal = useCallback(
    (repo) => {
      setUploadingName("");
      setUploadingPersonId("");
      setUploadedImages([]);
      setUploadingRepo(repo);
    },
    []
  )

  const handleUploadRepositoryModalOk = useCallback(
    async () => {
      if (!uploadingRepo) {
        message.warn('Code has some problem! [uploadingRepo is null]');
        return;
      };
      setUploadRepositoryModalOkLoading(true);
      try {
        const response = await insertRepositoryItemAPI({
          repoId: uploadingRepo.id,
          name: uploadingName,
          personId: uploadingPersonId,
          imageBase64: uploadingImage,
          detectScore: 0.5,
        });
  
        if (response.code !== 0) {
          message.error(response.message);
          return;
        }
  
        message.success(intl.formatMessage({
          id: 'pages.repository.upload.model.success',
          defaultMessage: '上传人像成功',
        }));
      } finally {
        setUploadRepositoryModalOkLoading(false);
      }
    },
    [intl, uploadingImage, uploadingName, uploadingPersonId, uploadingRepo]
  )

  const handleUploadRepositoryModalCancel = useCallback(
    () => {
      setUploadingRepo(null);
    },
    []
  )

  const onChangeUploadingName = useCallback(
    (e) => {
      setUploadingName(e.target.value);
    },
    []
  )

  const onChangeUploadingPersonId = useCallback(
    (e) => {
      setUploadingPersonId(e.target.value);
    },
    []
  )
  

  const uploadFaceProps: UploadProps = {
    accept: '.jpg, .jpeg, .png',
    maxCount: 1,
    showUploadList: true,
    fileList: uploadedImages,
    onRemove: file => {
      setUploadingImage("");
      setUploadedImages([]);
      console.log(file);
    },
    beforeUpload: file => {
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
        setUploadingImage(imgUri.replace(/^data:image\/\w+;base64,/, ""));
        setUploadedImages([file]);
      })
      return false;
    },
  };

  const columns: ColumnsType<RepositoryDataType> = [
    {
      title: intl.formatMessage({
        id: 'pages.repository.table.column.name',
        defaultMessage: '名字',
      }),
      dataIndex: 'name',
      render: (text: string, repo: RepositoryDataType) => <a onClick={() => history.push(`/repo_item?id=${repo.id}`)}>{text}</a>,
    },
    {
      title: intl.formatMessage({
        id: 'pages.repository.table.column.total',
        defaultMessage: '总数',
      }),
      dataIndex: 'total',
    },
    {
      title: intl.formatMessage({
        id: 'pages.repository.table.column.createTime',
        defaultMessage: '创建时间',
      }),
      dataIndex: 'createTime',
    },
    {
      title: intl.formatMessage({
        id: 'pages.repository.table.column.operation',
        defaultMessage: '操作',
      }),
      key: 'operation',
      align: 'center',
      fixed: 'right',
      width: 200,
      render: (repo: RepositoryDataType) => 
        <div className={styles.columnOperation}>
          <a onClick={() => openUploadRepositoryModal(repo)}>{
              intl.formatMessage({
                id: 'pages.repository.table.column.operation.upload',
                defaultMessage: '上传',
              })
            }
          </a>
          <Popconfirm
            title={
              intl.formatMessage({
                id: 'pages.repository.deleteRepository.popconfirm',
                defaultMessage: '确认是否删除人像库？',
              })
            }
            onConfirm={() => onClickDeletRepository(repo)}
            okText={
              intl.formatMessage({
                id: 'pages.repository.popconfirm.ensure',
                defaultMessage: '确认',
              })
            }
            cancelText={
              intl.formatMessage({
                id: 'pages.repository.popconfirm.cancel',
                defaultMessage: '取消',
              })
            }
          >
            <a>{
              intl.formatMessage({
                id: 'pages.repository.table.column.operation.delete',
                defaultMessage: '删除',
              })
            }
            </a>
          </Popconfirm>
        </div>,
    },
  ];


  return (
    <PageContainer>
      <div className={styles.wrapper}>
        <div className={styles.header}>
          <Button
            onClick={onClickCreateRpositoryButton}
          >{
              intl.formatMessage({
                id: 'pages.repository.createRepository.Button',
                defaultMessage: '新建人像库',
              })
            }</Button>
        </div>
        <Table
          className={styles.listTable}
          columns={columns}
          dataSource={repositories}
        />
      </div>
      <Modal 
        className={styles.createRepositoryModal}
        title={
          intl.formatMessage({
            id: 'pages.repository.createRepository.model.title',
            defaultMessage: '新建人像库',
          })
        }
        visible={isCreateRepositoryModalVisible} 
        confirmLoading={createRepositoryModalOkLoading}
        onOk={handleCreateRepositoryModalOk} 
        onCancel={handleCreateRepositoryModalCancel}
      >
        <Input
          className={styles.createRepositoryModalInput}
          placeholder={
          intl.formatMessage({
            id: 'pages.repository.createRepository.model.inputNamePlaceholder',
            defaultMessage: '请输入人像库名字',
          })
          }
          onChange={onChangeCeateRepositoryModalNameInput}
          value={ceateRepositoryModalNameInput}
        />
      </Modal>
      <Modal 
        className={styles.uploadRepositoryModal}
        title={
          intl.formatMessage({
            id: 'pages.repository.upload.model.title',
            defaultMessage: '上传人像',
          }) + `(${uploadingRepo && uploadingRepo.name})`
        }
        visible={uploadingRepo !== null} 
        confirmLoading={uploadRepositoryModalOkLoading}
        onOk={handleUploadRepositoryModalOk} 
        onCancel={handleUploadRepositoryModalCancel}
      >
        <div>
          <Upload {...uploadFaceProps}>
            <Button className={styles.item} icon={<UploadOutlined />}>
              {intl.formatMessage({
              id: 'pages.repository.upload.model.uploadButton',
              defaultMessage: '点击上传',
              })}
            </Button>
          </Upload>
        </div>
        <div className={styles.item}>
          <Input
            placeholder={
            intl.formatMessage({
              id: 'pages.repository.upload.model.nameInput',
              defaultMessage: '姓名',
            })
            }
            onChange={onChangeUploadingName}
            value={uploadingName}
          />
        </div>
        <div className={styles.item}>
          <Input
            placeholder={
            intl.formatMessage({
              id: 'pages.repository.upload.model.personIdInput',
              defaultMessage: 'ID',
            })
            }
            onChange={onChangeUploadingPersonId}
            value={uploadingPersonId}
          />
        </div>
      </Modal>
    </PageContainer>
  );
};

export default Repository;