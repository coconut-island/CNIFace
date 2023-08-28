// @ts-ignore
/* eslint-disable */

declare namespace API {

  enum CNIFaceErrorCode {
    OK = 0,
  }

  type PageParams = {
    current?: number;
    pageSize?: number;
  };

  type CNIFaceResponse<T> = {
    code: number;
    message: string;
    result: T;
    total?: number; 
  }

  type ErrorResponse = {
    errorCode: string;
    errorMessage?: string;
    success?: boolean;
  };

  type CurrentUser = {
    username: string;
    authorities: string[];
  };

  type CurrentUserResponse = CNIFaceResponse<CurrentUser>;

  type LoginParams = {
    username?: string;
    password?: string;
    rememberMe?: boolean;
  };

  type LoginResult = {
    token: string;
    authorities?: string[];
  };

  type LoginResponse = CNIFaceResponse<LoginResult>;

  type FaceDetectParams = {
    faceImageBase64: string;
    score: number;
    isReturnRecognitionSubImage?: boolean;
  } 

  type FaceDetectResult = {
    score: number;
    x: number;
    y: number;
    w: number;
    h: number;
    kps: number[];
    subImageBsae64?: string;
  }

  type FaceDetectResponse = CNIFaceResponse<FaceDetectResult[]>;

  type ExtractFeatureParams = {
    faceImageBase64: string;
    kps?: number[]; 
  }

  type ExtractFeatureResult = {
    feature: number[];
  }

  type ExtractFeatureResponse = CNIFaceResponse<ExtractFeatureResult>;

  type SimilarityParams = {
    feature1: number[];
    feature2: number[];
  }

  type SimilarityResult = {
    similarity: number;
  }
  
  type SimilarityResponse = CNIFaceResponse<SimilarityResult>;

  type InitAndRegisterAdminParams = {
    password: string;
  }

  type InitAndRegisterAdminResponse = CNIFaceResponse;

  type RepositoryResult = {
    id: number;
    name: string;
    total: number;
    createTime: number;
  }
  
  type ListRepositoryResponse = CNIFaceResponse<RepositoryResult[]>;

  type CreateRepositoryParams = {
    name: string;
  }

  type CreateRepositoryResponse = CNIFaceResponse;

  type DeleteRepositoryParams = {
    repoId: number;
  }

  type DeleteRepositoryResponse = CNIFaceResponse;

  type InsertRepositoryItemParams = {
    repoId: number;
    id?: number;
    name: string;
    personId: string;
    imageBase64: string;
    detectScore: number;
  }

  type InsertRepositoryItemResponse = CNIFaceResponse;

  type DeleteRepositoryItemParams = {
    repoId: number;
    itemId: number;
  }

  type DeleteRepositoryItemResponse = CNIFaceResponse;

  type RetrievalRepositoryItemParams = {
    repoId: number;
    topk: number;
    imageBase64?: string;
    detectScore?: number;
    feature?: number[];
  }

  type RetrievalRepositoryItemResult = {
    id: number;
    name: string;
    personId: string;
    score: number;
    timestamp: number;
    feature?: number[];
  }

  type RetrievalRepositoryItemResponse = CNIFaceResponse<RetrievalRepositoryItemResult[]>;

  type QueryRepositoryItemParams = {
    repoId: number;
    name?: string;
    personId?: string;
    startTimestamp?: number;
    endTimestamp?: number;
    page: number;
    size: number;
  }

  type RepositoryQueryResult = {
    id: number;
    name: string;
    personId: string;
    feature?: number[];
    timestamp: number;
  }

  type QueryRepositoryItemResponse = CNIFaceResponse<RepositoryQueryResult[]>;
}
