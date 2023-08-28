// @ts-ignore
/* eslint-disable */
import { request } from 'umi';

export async function currentUserAPI(options?: { [key: string]: any }) {
  return request<API.CurrentUserResponse>('/api/currentUser', {
    method: 'GET',
    ...(options || {}),
  });
}

export async function loginAPI(body: API.LoginParams, options?: { [key: string]: any }) {
  return request<API.LoginResponse>('/api/login', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export async function faceDetectAPI(body: API.FaceDetectParams, options?: { [key: string]: any }) {
  return request<API.FaceDetectResponse>('/api/detect/face', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export async function extractFeatureAPI(body: API.ExtractFeatureParams, options?: { [key: string]: any }) {
  return request<API.ExtractFeatureResponse>('/api/recognition/extract_feature', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export async function similarityAPI(body: API.SimilarityParams, options?: { [key: string]: any }) {
  return request<API.SimilarityResponse>('/api/recognition/similarity', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export async function initAndRegisterAdminAPI(body: API.InitAndRegisterAdminParams, options?: { [key: string]: any }) {
  return request<API.InitAndRegisterAdminResponse>('/api/init_and_register_admin', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export async function listRepositoryAPI(options?: { [key: string]: any }) {
  return request<API.ListRepositoryResponse>('/api/repository/list', {
    method: 'GET',
    ...(options || {}),
  });
}

export async function createRepositoryAPI(body: API.CreateRepositoryParams, options?: { [key: string]: any }) {
  return request<API.CreateRepositoryResponse>('/api/repository/create', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export async function deleteRepositoryAPI(body: API.DeleteRepositoryParams, options?: { [key: string]: any }) {
  return request<API.DeleteRepositoryResponse>(`/api/repository/delete`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export async function insertRepositoryItemAPI(body: API.InsertRepositoryItemParams, options?: { [key: string]: any }) {
  return request<API.InsertRepositoryItemResponse>(`/api/repository/item/insert`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export async function deleteRepositoryItemAPI(body: API.DeleteRepositoryItemParams, options?: { [key: string]: any }) {
  return request<API.DeleteRepositoryItemResponse>(`/api/repository/item/delete`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export async function retrievalRepositoryItemAPI(body: API.RetrievalRepositoryItemParams, options?: { [key: string]: any }) {
  return request<API.RetrievalRepositoryItemResponse>(`/api/repository/item/retrieval`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export async function queryRepositoryItemAPI(body: API.QueryRepositoryItemParams, options?: { [key: string]: any }) {
  return request<API.QueryRepositoryItemResponse>(`/api/repository/item/query`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    data: body,
    ...(options || {}),
  });
}

export const FACE_IMAGE_URL = (repoId: number, itemId: number) => `/api/repository/item/face_image/${repoId}/${itemId}`;
export const SCENE_IMAGE_URL = (repoId: number, itemId: number) => `/api/repository/item/scene_image/${repoId}/${itemId}`;