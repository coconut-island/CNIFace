package cn.abellee.cniface.platform.service;

import cn.abellee.cniface.platform.domain.dto.DetectRequestDTO;
import cn.abellee.cniface.platform.domain.dto.DetectResult;

import java.util.List;

/**
 * @author abel
 * @date 2022/6/5 3:27 PM
 */
public interface IDetectService {

    List<DetectResult> detect(final DetectRequestDTO detectRequest);
}
